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

Adafruit_BMP280 bmp;
#ifdef BMP280_CHIPID 
#undef BMP280_CHIPID
#endif
#define BMP280_CHIPID 0x58

bool bmp280Setup() {
    if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
        logError("BMP280", "Could not find a valid BMP280 sensor, check wiring!");
        return false;
    }
    return true;
}

float bmp280ReadTemperature() {
    return bmp.readTemperature();
}

float bmp280ReadPressure() {
    return bmp.readPressure();
}
