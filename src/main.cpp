#include <Arduino.h>
#include <Wire.h>

#include "connections/WiFi.hpp"
#include "connections/MQTT.hpp"
#include "utilities/loggerLib.hpp"
#include "utilities/JSONUtils.hpp"

#include "sensors/aht20.hpp"
#include "sensors/bmp280.hpp"
#include "sensors/ens160.hpp"

#define SDA_PIN 41
#define SCL_PIN 42

void setup(){
	Serial.begin(115200);
	logInfo("MAIN", "Starting Setup");
	wifiSetup(true);
	// setupTime();
	// syncTimeServer();
	
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

void loop(){
	delay(5000);
	float temperature = random(20, 25);
	float humidity = random(50, 60);
	float pressure = random(1000, 1010);
	float x = random(11, 12);
	float y = random(44, 45);
	char *jsonMsg = serializeSensorData(&temperature, &humidity, &pressure, &x, &y);
	//char *jsonMsg = setJsonSensorData(temperature, humidity, pressure, x, y);
	// Serial.printf("Json to be pubblish: %s\n", jsonMsg);
	// publishData(jsonMsg);
	// Serial.printf("Json to be pubblish: \n", publishData(jsonMsg));
}