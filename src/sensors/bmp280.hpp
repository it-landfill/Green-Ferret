/**
 * @file bmp280.hpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef BMP280_HPP
#define BMP280_HPP

// #define DISABLE_BMP280

/**
 * @brief Setup BMP280 sensor.
 *
 * @return true If the setup succedes
 * @return false If the setup fails
 */
bool bmp280Setup();
/**
 * @brief Read the temperature from the BMP280 sensor.
 *
 * @return float The temperature in °C.
 */
float bmp280ReadTemperature();
/**
 * @brief Read the pressure from the BMP280 sensor.
 *
 * @return float The pressure in hPa.
 */
float bmp280ReadPressure();
#endif