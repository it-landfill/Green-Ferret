#include <Arduino.h>
#include <Wire.h>

#include "connections/WiFi.hpp"
#include "connections/MQTT.hpp"
#include "utilities/loggerLib.hpp"
#include "utilities/JSONUtils.hpp"

#include "sensors/aht20.hpp"
#include "sensors/bmp280.hpp"
#include "sensors/ens160.hpp"

// Switch between WROOM32 pin and HELTEC pin
#ifdef WROOM32
#define SDA_PIN 21
#define SCL_PIN 22
#else
#define SDA_PIN 41
#define SCL_PIN 42
#endif

void setup(){
	Serial.begin(115200);
	logInfo("MAIN", "Starting Setup");
	wifiSetup(true);

	// Connect to the remote broker and subscribe to the topic
	mqttSetup();
	mqttConnect();

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
	float temperature = bmp280ReadTemperature();
	float humidity = aht20GetHumidity();
	float lat = generateLatitute();
	float lon = generateLongitude();
	int aqi = ens160GetAQI();
	int tvoc = ens160GetTVOC();
	int eco2 = ens160GetECO2();
	char* jsonMsg = NULL;

	if (counter++ == 5) { // Every minute
		float pressure = bmp280ReadPressure();
		jsonMsg = serializeSensorData(&temperature, &humidity, &pressure, &lat, &lon, &aqi, &tvoc, &eco2);
		counter = 0;
	} else {
		jsonMsg = serializeSensorData(&temperature, &humidity, NULL, &lat, &lon, &aqi, &tvoc, &eco2);
	}
	logInfof("MAIN", "Json to be pubblish: %s", jsonMsg);
	mqttPublishSensorData(jsonMsg);
	free(jsonMsg);
	delay(10000); // Sleep for 10 seconds
}