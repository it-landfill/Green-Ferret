/**
 * @file JSONUtils.hpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef JSONUTILS_HPP
#define JSONUTILS_HPP

/**
 * @brief Serialize the sensor data to JSON. If a value is NULL it will not be added to the JSON string
 *
 * @param temperature temperature value
 * @param humidity humidity value
 * @param pressure pressure value
 * @param latitude latitude value
 * @param longitude longitude value
 * @param aqi aqi value
 * @param tvoc tvoc value
 * @param eco2 eco2 value
 * @return char* JSON string. MUST be freed after use.
 */
char* serializeSensorData(float *temperature=NULL, float *humidity=NULL, float *pressure=NULL, float *latitude=NULL, float *longitude=NULL, int *aqi=NULL, int *tvoc=NULL, int *eco2=NULL);
#endif