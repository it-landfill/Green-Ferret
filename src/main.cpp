#include <Arduino.h>
#include <Wire.h>

#include "connections/WiFi.hpp"
#include "connections/DataUpload.hpp"
#include "utilities/loggerLib.hpp"
#include "utilities/JSONUtils.hpp"
#include "utilities/gpsDistanceUtils.hpp"
#include "utilities/dataGPSStruct.hpp"

#include "sensors/aht20.hpp"
#include "sensors/bmp280.hpp"
#include "sensors/ens160.hpp"
#include "sensors/gps.hpp"

// Switch between WROOM32 pin and HELTEC pin
#ifdef WROOM32
#define SDA_PIN 21
#define SCL_PIN 22
#else
#define SDA_PIN 41
#define SCL_PIN 42
#endif

// #define LOCAL_DEBUG

// Set to 1 to publish based on distance, 0 to publish based on time
#define sendConditionSet 0

// Set the distance method to use
// 0: Naive
// 1: Haversine
// 2: Vincenty
// 3: Spherical Law of Cosines
#define distanceMethodSet 0

float distance = 0;

void setup() {
	Serial.begin(115200);
	logInfo("MAIN", "Starting Setup");

#ifndef LOCAL_DEBUG
	wifiSetup(true);
	dataUploadSetup(HTTP);
#else
	logWarning("MAIN", "Local Debug Enabled");
#endif

	// Sensor setup
	Wire.begin(SDA_PIN, SCL_PIN);
	aht20Setup();
	ens160Setup(aht20GetTemperature(), aht20GetHumidity());
	bmp280Setup();

	// GPS setup
	gpsSetup();

	// TODO: Create cpp file for settings
	// TODO: Get sendConditionSet from MQTT broker
	logInfof("SETTINGS", "Send condition set to %d", sendConditionSet);

	logInfo("MAIN", "Setup Complete");
}

int counter = 0;

// Generate a random latitude between 45.6 and 45.7
float generateLatitute() {
	float lat = 45.6;
	lat += (float)(rand() % 10000 + 1) / 100000;
	return lat;
}
// Generate a random longitude between 12.2 and 12.3
float generateLongitude() {
	// Between 12.2 and 12.3
	float lon = 12.2;
	lon += (float)(rand() % 10000 + 1) / 100000;
	return lon;
}

void loop() {

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
		// Publish based on time
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