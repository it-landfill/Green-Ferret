#include <Arduino.h>
#include <Wire.h>

#include "connections/WiFi.hpp"
#include "connections/MQTT.hpp"
#include "utilities/loggerLib.hpp"
#include "utilities/timeLib.hpp"

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
    connectMQTT(); 
    subscribeTopicMQTT(genConfigChannel());
	// Set the data channel
	setDataChannel();

	// Sensor setup
	Wire.begin(SDA_PIN, SCL_PIN);
	aht20Setup();
	ens160Setup(aht20GetTemperature(), aht20GetHumidity());
	bmp280Setup();

	logInfo("MAIN", "Setup Complete");
}

void loop(){
	delay(5000);
	float temperature = random(20, 30);
	char temperatureChar[10];
	dtostrf(temperature, 4, 2, temperatureChar);
	Serial.printf("Temperature pubblish result: %d \n", publishData(strdup("Cresp/data"), temperatureChar));
}