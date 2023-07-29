/**
 * @file gpsDistanceUtils.hpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef GPSDISTANCEUTILS_HPP
#define GPSDISTANCEUTILS_HPP

#include "dataGPSStruct.hpp"

float getDistanceNaive(struct gpsPoint p1, struct gpsPoint p2);
float getDistanceHaversine(struct gpsPoint p1, struct gpsPoint p2);
float getDistanceVincenty(struct gpsPoint p1, struct gpsPoint p2);
float getDistanceSphericalLawOfCosines(struct gpsPoint p1, struct gpsPoint p2);

#endif