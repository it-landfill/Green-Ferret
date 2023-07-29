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

// GPS Point struct to store the last point
struct gpsPoint {
    float lat;
    float lon;
    long timestamp;
};

#endif