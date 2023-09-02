/**
 * @file bmp280.cpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Arduino.h"
#include <Wire.h>

#include <Adafruit_BMP280.h>

#include "../bmp280.hpp"
#include "../../utilities/loggerLib.hpp"

#define MODULE_NAME "BMP280"

Adafruit_BMP280 bmp;
#ifdef BMP280_CHIPID
#undef BMP280_CHIPID
#endif
#define BMP280_CHIPID 0x58

bool bmp280Setup() {
	#ifndef DISABLE_BMP280
	logDebug(MODULE_NAME, "Begin setup");
	if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
		logError("BMP280", "Could not find a valid BMP280 sensor, check wiring!");
		return false;
	} else {
		logInfo(MODULE_NAME, "BMP280 sensor found!");
	}
	#else
	logWarning(MODULE_NAME, "BMP280 disabled");
	#endif
	return true;
}




/*
	https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf (page 25)

   https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
   endTransmission() returns:
   0: success.
   1: data too long to fit in transmit buffer.
   2: received NACK on transmit of address.
   3: received NACK on transmit of data.
   4: other error.
   5: timeout
 */
bool bmp280Ping() {
	#ifndef DISABLE_BMP280
	Wire.beginTransmission(BMP280_ADDRESS_ALT);
	byte error = Wire.endTransmission();
	// No error, nice
	if (error == 0) return true;

	// Well... not so good, but could be worse
	logError(MODULE_NAME, "Sensor not responding, ping returned:", String(error));
	return false;
	#else
	return true;
	#endif
}

float bmp280ReadTemperature() {
	#ifndef DISABLE_BMP280
	if (bmp280Ping()) return bmp.readTemperature();
	else return -1.1;
	#else
	return -1.1;
	#endif
}

float bmp280ReadPressure() {
	#ifndef DISABLE_BMP280
	if (bmp280Ping()) return bmp.readPressure();
	else return -1.1;
	#else
	return -1.1;
	#endif
}
