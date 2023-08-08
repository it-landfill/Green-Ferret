/**
 * @file dataGPSStruct.hpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DATAGPSSTRUCT_HPP
#define DATAGPSSTRUCT_HPP

/**
 * @brief A struct to store GPS data
 * 
 * @param lat The latitude
 * @param lon The longitude
 * @param timestamp The timestamp in epoch format
 */
struct gpsPoint {
    float lat;
    float lon;
    long timestamp;
};

#endif