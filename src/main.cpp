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

// Initialize the last point to 0
struct gpsPoint lastPoint = {0, 0, 0};
// Set the minimum distance to 0.1 km (100 m) to publish
#define minDistance 0.1
// Set to 1 to publish based on distance, 0 to publish based on time
#define sendConditionSet 0

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

void loop() {

	dataUploadLoop();

	float lat = generateLatitute();
	float lon = generateLongitude();

	// Get sensor data
	float temperature = bmp280ReadTemperature();
	float humidity = aht20GetHumidity();
	int aqi = ens160GetAQI();
	int tvoc = ens160GetTVOC();
	int eco2 = ens160GetECO2();

	// Get latitute and longitude from GPS
	getLocation();
	// Get the new point
	struct gpsPoint point = getNewPoint();

	char* jsonMsg = NULL;

	if (sendConditionSet) {
		// Publish based on distance travelled since last publish
		// Based on the distance method set, calculate the distance between the 
		// last point and the new point with the selected method.
		switch (distanceMethodSet) {
			case 0:
				distance = getDistanceNaive(getLastPoint(), getNewPoint());
				break;
			case 1:
				distance = getDistanceNaive(getLastPoint(), getNewPoint());
				break;
			case 2:
				distance = getDistanceVincenty(getLastPoint(), getNewPoint());
				break;
			case 3:
				distance = getDistanceSphericalLawOfCosines(getLastPoint(), getNewPoint());
				break;
			default:
				distance = getDistanceNaive(getLastPoint(), getNewPoint());
				break;
		}
		// If the distance is greater than the minimum distance or the last point is not set, publish the data.
		if (distance > getMinDistance() || getLastPoint().timestamp == 0) {
			// Publish the data
			jsonMsg = serializeSensorData(&temperature, &humidity, &temperature, &point.lat, &point.lon, &aqi, &tvoc, &eco2);
			// Update the last point
			updateGPSPoint();
		} else {
			// Calculate the time needed to reach the minDistance based on the speed and the distance already travelled
			long timeToReach = (long)((getMinDistance() - distance) / getSpeed());
			// Calculate the millis to sleep
			long timeToSleep = timeToReach + millis();
			delay(timeToSleep);
		}
	} else {
		// Publish based on time elapsed since last publish
		if (counter++ == 5) {
			// Every minute
			float pressure = bmp280ReadPressure();
			jsonMsg = serializeSensorData(&temperature, &humidity, &pressure, &point.lat, &point.lon, &aqi, &tvoc, &eco2);
			counter = 0;
		} else {
			jsonMsg = serializeSensorData(&temperature, &humidity, NULL, &point.lat, &point.lon, &aqi, &tvoc, &eco2);
		}
		logInfof("MAIN", "Json to be pubblish: %s", jsonMsg);
#ifndef LOCAL_DEBUG
		publishSensorData(jsonMsg);
#endif
		free(jsonMsg);
#ifdef LOCAL_DEBUG
		delay(1000);
#else
		delay(10000); // Sleep for 10 seconds
#endif
	}
}