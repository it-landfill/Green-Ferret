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
#include <SPI.h>
#include <Adafruit_BMP280.h>

#include "../../utilities/loggerLib.hpp"

#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)

Adafruit_BMP280 bmp; // I2C
// Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
// Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

bool bmp280Setup() {
    if (!bmp.begin()) {
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
