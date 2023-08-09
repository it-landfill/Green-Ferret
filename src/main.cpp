#include <Arduino.h>
#include <Wire.h>

#include "connections/WiFi.hpp"
#include "connections/DataUpload.hpp"
#include "utilities/loggerLib.hpp"
#include "utilities/JSONUtils.hpp"
#include "utilities/gpsDistanceUtils.hpp"
#include "utilities/dataGPSStruct.hpp"
#include "utilities/randomGPS.hpp"

#include "sensors/aht20.hpp"
#include "sensors/bmp280.hpp"
#include "sensors/ens160.hpp"
#include "sensors/gps.hpp"

#include "memory/settings.hpp"

// Switch between WROOM32 pin and HELTEC pin
#ifdef WROOM32
	#define SDA_PIN 21
	#define SCL_PIN 22
#else
	#define SDA_PIN 41
	#define SCL_PIN 42
#endif

// #define LOCAL_DEBUG

// Connection settings reference.
ConnectionSettings connSettings;

// Data upload settings reference.
Settings settings = {
	.protocol = NONE,
	.trigger = -1,
	.distance = -1,
	.time = -1
};

// Counter for sent messages interval.
int counter = 0;

// TODO: Add distanceMethodSet to settings
// Set the distance method to use
// 0: Naive
// 1: Haversine
// 2: Vincenty
// 3: Spherical Law of Cosines
#define distanceMethodSet 0

float distance = 0;

// Setup ESP32 
void setup(){
	// Init serial baud rate
	Serial.begin(115200);
	
	logInfo("MAIN", "Starting Setup");

	// Load settings from memory and save them.
	connectionSettingsInit(&connSettings);
	connectionSettingsSave();

	#ifndef LOCAL_DEBUG
		// Setup connections and data upload
		wifiInit(&connSettings);
		wifiSetup();
		dataUploadSetup(&settings, &connSettings);
	#else
		logWarning("MAIN", "Local Debug Enabled");
	#endif

	// Setup sensors. In order: 
	// 1. AHT20
	// 2. ENS160
	// 3. BMP280
	// 4. GPS
	Wire.begin(SDA_PIN, SCL_PIN);
	aht20Setup();
	ens160Setup(aht20GetTemperature(), aht20GetHumidity());
	bmp280Setup();
	gpsSetup();

	logInfo("MAIN", "Setup Complete");
}

// Main loop
void loop() {

	// Mantain the connection with the broker MQTT
	dataUploadLoop();

	// Get temperature and pressure from BMP280
	float temperature = bmp280ReadTemperature();
	float pressure = bmp280ReadPressure();
	// Get humidity from AHT20
	float humidity = aht20GetHumidity();
	// Get AQI, TVOC and eCO2 from ENS160
	int aqi = ens160GetAQI();
	int tvoc = ens160GetTVOC();
	int eco2 = ens160GetECO2();
	// Get GPS data
	struct gpsPoint point = getNewPoint();

	// Init the JSON message to NULL
	char* jsonMsg = NULL;

	if (settings.trigger == 0) {
		// Publish based on distance travelled since last publish.
		// Based on the distance method set, calculate the distance between the last point and the new point with the selected method.
		distance = getDistance(static_cast<DistanceMethod>(distanceMethodSet), getLastPoint(), getNewPoint());
		// If the distance is greater than the minimum distance or the last point is not set, publish the message.
		if (distance > settings.distance || getLastPoint().timestamp == 0) {
			jsonMsg = serializeSensorData(&temperature, &humidity, &temperature, &point.lat, &point.lon, &aqi, &tvoc, &eco2);
			// Update the last point with the new point.
			updateGPSPoint();
		}
	} else {
		// Publish based on time elapsed since last publish.
		if (counter++ == 5) {
			jsonMsg = serializeSensorData(&temperature, &humidity, &pressure, &point.lat, &point.lon, &aqi, &tvoc, &eco2);
			counter = 0;
		} 
	}
	// If the condition is not met, publish the message without the pressure.
	if (jsonMsg == NULL) jsonMsg = serializeSensorData(&temperature, &humidity, NULL, &point.lat, &point.lon, &aqi, &tvoc, &eco2);
	logInfof("MAIN", "Json to be pubblish: %s", jsonMsg);
	// Publish the JSON message, if local debug is not enabled.
	#ifndef LOCAL_DEBUG
		publishSensorData(jsonMsg);
	#endif
	// Free the JSON message
	free(jsonMsg);
	// If local debug is enabled, wait 10 seconds before sending another message, otherwise sleep for 1 seconds.
	#ifdef LOCAL_DEBUG
		delay(1000);
	#else
		delay(10000);
	#endif	
}