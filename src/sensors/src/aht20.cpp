#include <Arduino.h>
#include <AHT20.h>

#include "../aht20.hpp"
#include "../../utilities/loggerLib.hpp"

#define MODULE_NAME "AHT20"
#define MESAUREMENT_INTERVAL_MS 2000

AHT20 aht20;
unsigned long lastMeasurement = 0;

bool aht20Setup(){
	logDebug(MODULE_NAME,"Begin setup");

	if (aht20.begin() == false)  {
		logError(MODULE_NAME, "AHT20 not detected. Please check wiring.");
		return false;
	}
	
	logDebug(MODULE_NAME, "Sensor initialized correctly");

	logDebug(MODULE_NAME, "End setup");
	return true;
}

void refreshMeasurement(){
	if (millis()>lastMeasurement+MESAUREMENT_INTERVAL_MS) {
		while (!aht20.available()) delay(100);
		lastMeasurement = millis();
	}
}

float aht20GetTemperature(){
	refreshMeasurement();
	return aht20.getTemperature();
}

float aht20GetHumidity(){
	refreshMeasurement();
	return aht20.getHumidity();
}