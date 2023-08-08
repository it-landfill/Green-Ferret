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
 * @brief Get the distance beetwen two GPS points using the naive formula. 
 * 
 * @param p1 The first GPS point
 * @param p2 The second GPS point
 * @return float The distance between the two points in km
 */
float getDistanceNaive(struct gpsPoint p1, struct gpsPoint p2);
/**
 * @brief Get the distance beetwen two GPS points using the haversine formula.
 *
 * @param p1 The first GPS point
 * @param p2 The second GPS point
 * @return float The distance between the two points in km
 */
float getDistanceHaversine(struct gpsPoint p1, struct gpsPoint p2);
/**
 * @brief Get the distance beetwen two GPS points using the vincenty formula.
 *
 * @param p1 The first GPS point
 * @param p2 The second GPS point
 * @return float The distance between the two points in km
 */
float getDistanceVincenty(struct gpsPoint p1, struct gpsPoint p2);
/**
 * @brief Get the distance beetwen two GPS points using the spherical law of cosines formula.
 *
 * @param p1 The first GPS point
 * @param p2 The second GPS point
 * @return float The distance between the two points in km
 */
float getDistanceSphericalLawOfCosines(struct gpsPoint p1, struct gpsPoint p2);

#endif