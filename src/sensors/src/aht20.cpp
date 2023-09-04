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

/*
   https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
   endTransmission() returns:
   0: success.
   1: data too long to fit in transmit buffer.
   2: received NACK on transmit of address.
   3: received NACK on transmit of data.
   4: other error.
   5: timeout
 */
bool aht20Ping() {
	#ifndef DISABLE_AHT20
	
	// No error, nice
	if (aht20.isConnected()) return true;

	// Well... not so good, but could be worse
	logError(MODULE_NAME, "Sensor not responding");
	return false;
	#else
	return true;
	#endif
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
	if (!aht20Ping()) return -1.1;
	refreshMeasurement();
	return aht20.getTemperature();
	#else
	return -1.1;
	#endif
}

float aht20GetHumidity(){
	#ifndef DISABLE_AHT20
	if (!aht20Ping()) return -1.1;
	refreshMeasurement();
	return aht20.getHumidity();
	#else
	return -1.1;
	#endif
}