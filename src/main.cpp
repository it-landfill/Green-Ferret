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
	.distanceMethod = NAIVE,
	.distance = -1,
	.time = -1
};

// Counter for sent messages interval.
int counter = 0;

// Counter time
unsigned long currentTime = 0;
unsigned long sendCounter = 1;

// Setup ESP32
void setup(){
	// Init serial baud rate
	Serial.begin(115200);

	logInfo("MAIN", "Starting Setup");

	// Load settings from memory and save them.
	connectionSettingsInit(&connSettings);
	rebootCountInit();
	rebootCountIncrease();

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
	gpsSetup(); // TODO: Enable GPS

	logInfo("MAIN", "Setup Complete");
}

// Main loop
void loop() {
	// Mantain the connection with the broker MQTT
	dataUploadLoop();

	// Update time
	currentTime = millis();

	// Get GPS data
	struct gpsPoint point = getNewPoint();
	float distanceTravelled = getDistance(settings.distanceMethod, getLastPoint(), getNewPoint());

	if((settings.trigger == 0 && (distanceTravelled >= settings.distance || getLastPoint().timestamp == 0)) || (settings.trigger == 1 && currentTime >= settings.time*1000*sendCounter)) {
		// Get temperature and pressure from BMP280
		float temperature = bmp280ReadTemperature();
		float pressure = bmp280ReadPressure();
		// Get humidity from AHT20
		float humidity = aht20GetHumidity();

		char *jsonMsg;

		// Only publish air quality data if the sensor is ready and operational.
		if (uint8_t ensStat = ens160GetStatus() == 0) {
			// Get AQI, TVOC and eCO2 from ENS160
			int aqi = ens160GetAQI();
			int tvoc = ens160GetTVOC();
			int eco2 = ens160GetECO2();
			jsonMsg = serializeSensorData(&temperature, &humidity, &pressure, &point.lat, &point.lon, &aqi, &tvoc, &eco2);
		} else {
			logInfo("MAIN", "ENS160 not ready, code:", ensStat);
			jsonMsg = serializeSensorData(&temperature, &humidity, &pressure, &point.lat, &point.lon, NULL, NULL, NULL);
		}

		// Update the last point with the new point.
		if(settings.trigger == 0) updateGPSPoint();
		else sendCounter++;

		if (jsonMsg != NULL) {
			logInfo("MAIN", "Json to be pubblish:", jsonMsg);
			// Publish the JSON message, if local debug is not enabled.
			#ifndef LOCAL_DEBUG
			publishSensorData(jsonMsg);
			#endif
			// Free the JSON message
			free(jsonMsg);
		}
	}
	// TODO: Increase? Decrease? Delay?
	delay(100);
}