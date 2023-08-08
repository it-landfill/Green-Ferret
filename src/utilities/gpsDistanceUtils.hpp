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

float getDistanceNaive(float lat1, float lon1, float lat2, float lon2);

float getDistanceHaversine(float lat1, float lon1, float lat2, float lon2);

float getDistanceVincenty(float lat1, float lon1, float lat2, float lon2);

float getDistanceSphericalLawOfCosines(float lat1, float lon1, float lat2, float lon2);

#endif