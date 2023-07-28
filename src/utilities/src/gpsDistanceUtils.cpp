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

// Earth's radius in km 
#define R 6371
// Degrees to radians conversion factor
#define DEG_TO_RAD 0.0174533

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

// Haversine formula
// Treats the globe as a sphere, more accurate than the naive formula but slower.
// Test: Distance between Big Ben in London (51.5007° N, 0.1246° W) and The Statue of Liberty in  New York (40.6892° N, 74.0445° W) is 5574.8 km.
float getDistanceHaversine(float lat1, float lon1, float lat2, float lon2) {
	// Calculate the difference between the two longitudes and latitudes
	float dLat = (lat2 - lat1) * DEG_TO_RAD;
	float dLon = (lon2 - lon1) * DEG_TO_RAD;
	// Convert the latitudes to radians
	float lat1Rad = lat1 * DEG_TO_RAD;
	float lat2Rad = lat2 * DEG_TO_RAD;
	// Calculate the haversine of the central angle: a = sin^2(dLat/2) + sin^2(dLon/2) * cos(lat1) * cos(lat2)
	float harvestineAngle = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1Rad) * cos(lat2Rad);
	// Calculate the distance: distance = 2 * arcsin(sqrt(harvestineAngle))
	float distance = 2 * asin(sqrt(harvestineAngle));
	// Return the distance in km
	return R * distance;
}

// Vincenty formula
// Treats the globe as an ellipsoid, the most accurate but also the slowest formula.
// Test: Distance between Big Ben in London (51.5007° N, 0.1246° W) and The Statue of Liberty in  New York (40.6892° N, 74.0445° W) is 5574.8 km.
float getDistanceVincenty(float lat1, float lon1, float lat2, float lon2) {
	// Calculate the difference between the two longitudes and latitudes
	float dLat = (lat2 - lat1) * DEG_TO_RAD;
	float dLon = (lon2 - lon1) * DEG_TO_RAD;
	// Convert the latitudes to radians
	float lat1Rad = lat1 * DEG_TO_RAD;
	float lat2Rad = lat2 * DEG_TO_RAD;
	// Calculate the numerator and denominator of the formula
	float numerator = sqrt(pow(cos(lat2Rad) * sin(dLon), 2) + pow(cos(lat1Rad) * sin(lat2Rad) - sin(lat1Rad) * cos(lat2Rad) * cos(dLon), 2));
	float denominator = sin(lat1Rad) * sin(lat2Rad) + cos(lat1Rad) * cos(lat2Rad) * cos(dLon);
	// Calculate the central angle
	float centralAngle = atan2(numerator, denominator);
	// Calculate the distance: distance = R * centralAngle
	float distance = R * centralAngle;
	// Return the distance in km
	return distance;
}

