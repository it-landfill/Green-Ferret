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

/**
 * @brief List of supported protocols for data upload.
 *
 */
enum DistanceMethod {
    NAIVE,
    HAVERSINE,
    VINCENTY,
    SPHERICALLAWOFCOSINES
};

/**
 * @brief Get the Distance object
 *
 * @param method Method to use (NAIVE, HAVERSINE, VINCENTY, SPHERICALLAWOFCOSINES)
 * @param p1 The first GPS point
 * @param p2 The second GPS point
 * @return float The distance between the two points in km
 */
float getDistance(DistanceMethod method, struct gpsPoint p1, struct gpsPoint p2);

#endif