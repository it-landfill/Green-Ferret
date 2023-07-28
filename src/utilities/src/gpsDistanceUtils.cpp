/**
 * @file gpsDistanceUtils.cpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../gpsDistanceUtils.hpp"

// Naive formula
// Treats the globe as a flat surface, the least accurate but also the fastest formula.
// Test: Distance between Big Ben in London (51.5007° N, 0.1246° W) and The Statue of Liberty in  New York (40.6892° N, 74.0445° W) is 5574.8 km.
float getDistanceNaive(float lat1, float lon1, float lat2, float lon2) {
	// Calculate the difference between the two longitudes and latitudes
	float dLat = (lat2 - lat1);
	float dLon = (lon2 - lon1);
	// Calculate and return the distance: distance = sqrt(dLat^2 + dLon^2)
	return sqrt(pow(dLat, 2) + pow(dLon, 2));
}
