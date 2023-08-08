#include <Arduino.h>
#include <Wire.h>

#include "connections/WiFi.hpp"
#include "connections/DataUpload.hpp"
#include "utilities/loggerLib.hpp"
#include "utilities/JSONUtils.hpp"
#include "utilities/gpsDistanceUtils.hpp"

#include "sensors/aht20.hpp"
#include "sensors/bmp280.hpp"
#include "sensors/ens160.hpp"

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
	Wire.begin(SDA_PIN, SCL_PIN);
	aht20Setup();
	ens160Setup(aht20GetTemperature(), aht20GetHumidity());
	bmp280Setup();

	logInfo("MAIN", "Setup Complete");
}



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

	dataUploadLoop();

	float lat = generateLatitute();
	float lon = generateLongitude();

	// Get sensor data
	float temperature = bmp280ReadTemperature();
	float humidity = aht20GetHumidity();
	int aqi = ens160GetAQI();
	int tvoc = ens160GetTVOC();
	int eco2 = ens160GetECO2();

	// Serialize the data
	char* jsonMsg = NULL;

	if (sendConditionSet) {

		// Publish based on distance

		// Create a new point
		// TODO: Use the GPS module to get the position.
		struct gpsPoint newPoint = { lat, lon, millis() };

		switch (distanceMethodSet) {
			case 0:
				distance = getDistanceNaive(lastPoint.lat, lastPoint.lon, newPoint.lat, newPoint.lon);
				break;
			case 1:
				distance = getDistanceHaversine(lastPoint.lat, lastPoint.lon, newPoint.lat, newPoint.lon);
				break;
			case 2:
				distance = getDistanceVincenty(lastPoint.lat, lastPoint.lon, newPoint.lat, newPoint.lon);
				break;
			case 3:
				distance = getDistanceSphericalLawOfCosines(lastPoint.lat, lastPoint.lon, newPoint.lat, newPoint.lon);
				break;
			default:
				distance = getDistanceHaversine(lastPoint.lat, lastPoint.lon, newPoint.lat, newPoint.lon);
				break;
		}

		if (distance > minDistance) {
			// Update the last point
			lastPoint.lat = lat;
			lastPoint.lon = lon;
			lastPoint.timestamp = millis();
			// Publish the data
			jsonMsg = serializeSensorData(&temperature, &humidity, &temperature, &lat, &lon, &aqi, &tvoc, &eco2);
		} else {
			// calculate the mean speed between the two points
			float speed = getDistanceHaversine(lastPoint.lat, lastPoint.lon, newPoint.lat, newPoint.lon) / (newPoint.timestamp - lastPoint.timestamp);
			// calculate the time needed to reach the minDistance
			long timeToReach = minDistance / speed;
			// calculate the time to sleep
			long timeToSleep = timeToReach - (millis() - lastPoint.timestamp);
			logInfof("MAIN", "Sleeping for %ld ms", timeToSleep);
			delay(timeToSleep);
		}
		
	} else {
		// Publish based on time
		if (counter++ == 5) {
			// Every minute
			float pressure = bmp280ReadPressure();
			jsonMsg = serializeSensorData(&temperature, &humidity, &pressure, &lat, &lon, &aqi, &tvoc, &eco2);
			counter = 0;
		} else {
			jsonMsg = serializeSensorData(&temperature, &humidity, NULL, &lat, &lon, &aqi, &tvoc, &eco2);
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