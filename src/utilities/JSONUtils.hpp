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
 * @brief Serialize the sensor data into a JSON string. Parameters are passed as pointers and omitted if NULL.
 * 
 * @param temperature (float*)
 * @param humidity (float*)
 * @param pressure (float*)
 * @param latitude (float*)
 * @param longitude (float*)
 * @param aqi (int*)
 * @param tvoc (int*)
 * @param eco2 (int*)
 * @return (char*) The JSON string. MUST be freed after use.
 */
char* serializeSensorData(float *temperature, float *humidity, float *pressure, float *latitude, float *longitude, int *aqi, int *tvoc, int *eco2);
#endif