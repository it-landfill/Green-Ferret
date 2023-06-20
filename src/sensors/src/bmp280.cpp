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

#include <Adafruit_BMP280.h>

#include "../../utilities/loggerLib.hpp"

#define MODULE_NAME "BMP280"

Adafruit_BMP280 bmp;
#ifdef BMP280_CHIPID 
#undef BMP280_CHIPID
#endif
#define BMP280_CHIPID 0x58

bool bmp280Setup() {
	#ifndef DISABLE_BMP280
    if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
        logError("BMP280", "Could not find a valid BMP280 sensor, check wiring!");
        return false;
    }
	#else
	logWarning(MODULE_NAME, "BMP280 disabled");
	#endif
    return true;
}

float bmp280ReadTemperature() {
	#ifndef DISABLE_BMP280
    return bmp.readTemperature();
	#else
	return -1.0;
	#endif
}

float bmp280ReadPressure() {
	#ifndef DISABLE_BMP280
    return bmp.readPressure();
	#else
	return -1.0;
	#endif
}
