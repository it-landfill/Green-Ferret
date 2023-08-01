#include <Arduino.h>
#include <Wire.h>

#include "connections/WiFi.hpp"
#include "connections/DataUpload.hpp"
#include "utilities/loggerLib.hpp"
#include "utilities/JSONUtils.hpp"

#include "sensors/aht20.hpp"
#include "sensors/bmp280.hpp"
#include "sensors/ens160.hpp"

#include "settings/settings.hpp"

// Switch between WROOM32 pin and HELTEC pin
#ifdef WROOM32
#define SDA_PIN 21
#define SCL_PIN 22
#else
#define SDA_PIN 41
#define SCL_PIN 42
#endif

//#define LOCAL_DEBUG

Settings settings = {
	.protocol = NONE,
	.trigger = -1,
	.distance = -1,
	.time = -1
};

void setup(){
	Serial.begin(115200);
	logInfo("MAIN", "Starting Setup");

	#ifndef LOCAL_DEBUG
	wifiSetup(true);
	dataUploadSetup(&settings, MQTT);
	#else
	logWarning("MAIN", "Local Debug Enabled");
	#endif

	// Sensor setup
	Wire.begin(SDA_PIN, SCL_PIN);
	aht20Setup();
	ens160Setup(aht20GetTemperature(), aht20GetHumidity());
	bmp280Setup();

	logInfo("MAIN", "Setup Complete");
}

int counter = 0;

float generateLatitute() {
	// Between 45.6 and 45.7
	float lat = 45.6;
	lat += (float) (rand() % 10000 + 1) / 100000;
	return lat;
}

float generateLongitude() {
	// Between 12.2 and 12.3
	float lon = 12.2;
	lon += (float) (rand() % 10000 + 1) / 100000;
	return lon;
}

void loop(){

	float lat = generateLatitute();
	float lon = generateLongitude();

	float humidity = aht20GetHumidity();
	int aqi = ens160GetAQI();
	int tvoc = ens160GetTVOC();
	int eco2 = ens160GetECO2();


	float temperature = bmp280ReadTemperature();

	char* jsonMsg = NULL;

	if (counter++ == 5) { // Every minute
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