/**
 * @file timeLib.cpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../randomGPS.hpp"

#include <Arduino.h>

float generateLatitute() {
    float lat = 45.6;
    lat += (float)(rand() % 10000 + 1) / 100000;
    return lat;
}

float generateLongitude() {
    // Between 12.2 and 12.3
    float lon = 12.2;
    lon += (float)(rand() % 10000 + 1) / 100000;
    return lon;
}