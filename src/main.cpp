#include <Arduino.h>
#include <Wire.h>

#include "connections/WiFi.hpp"
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
	// wifiSetup(true);
	// setupTime();
	// syncTimeServer();

	// Sensor setup
	Wire.begin(SDA_PIN, SCL_PIN);
	aht20Setup();
	ens160Setup(aht20GetTemperature(), aht20GetHumidity());
	bmp280Setup();

	logInfo("MAIN", "Setup Complete");
}

void loop(){
	delay(1000);
}