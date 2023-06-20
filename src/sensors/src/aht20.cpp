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

	#ifndef DISABLE_AHT20
	if (aht20.begin() == false)  {
		logError(MODULE_NAME, "AHT20 not detected. Please check wiring.");
		return false;
	}
	#else
	logWarning(MODULE_NAME, "AHT20 disabled");
	#endif
	
	logDebug(MODULE_NAME, "Sensor initialized correctly");

	logDebug(MODULE_NAME, "End setup");
	return true;
}

void refreshMeasurement(){
	#ifndef DISABLE_AHT20
	if (millis()>lastMeasurement+MESAUREMENT_INTERVAL_MS) {
		while (!aht20.available()) delay(100);
		lastMeasurement = millis();
	}
	#endif
}

float aht20GetTemperature(){
	#ifndef DISABLE_AHT20
	refreshMeasurement();
	return aht20.getTemperature();
	#else
	return -1.0;
	#endif
}

float aht20GetHumidity(){
	#ifndef DISABLE_AHT20
	refreshMeasurement();
	return aht20.getHumidity();
	#else
	return -1.0;
	#endif
}