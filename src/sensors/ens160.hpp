#ifndef ENS160_HPP
#define ENS160_HPP
#include <Arduino.h>
#include <DFRobot_ENS160.h>

// #define DISABLE_ENS160

/**
 * @brief  Initial configuration for ENS160 sensor
 *
 * @param ambientTemp Ambient temperature (necessary to calibrate sensor)
 * @param ambientHum  Ambient humidity (necessary to calibrate sensor)
 * @param mode The mode of operation for the sensor (see ens160SetPWRMode())
 * @return true If the setup succedes
 * @return false If the setup fails
 */
bool ens160Setup(float ambientTemp, float ambientHum, uint8_t mode=ENS160_STANDARD_MODE);

/**
 * @brief Set the power mode of the sensor
 *
 * @param mode The mode of operation for the sensor
 *   ENS160_SLEEP_MODE: DEEP SLEEP mode (low power standby)
 *   ENS160_IDLE_MODE: IDLE mode (low-power)
 *   ENS160_STANDARD_MODE: STANDARD Gas Sensing Modes
 */
void ens160SetPWRMode(uint8_t mode);

/**
 * @brief Set the temperature and humidity of the environment
 *
 * @param ambientTemp Ambient temperature (necessary to calibrate sensor)
 * @param ambientHum  Ambient humidity (necessary to calibrate sensor)
 */
void setTempAndHum(float ambientTemp, float relativeHumidity);

/**
 * @brief Get the sensor operating status
 * @return uint8_t The status of the sensor
 * 		0-Normal operation,
 *      1-Warm-Up phase, first 3 minutes after power-on.
 *      2-Initial Start-Up phase, first full hour of operation after
 * 		initial power-on. Only once in the sensor’s lifetime.
 */
uint8_t ens160GetStatus();

/**
 * @brief Get the Air Quality Index
 * @return uint8_t The AQI:
 * - 0 Initialization in progress
 * - 1 Excellent
 * - 2 Good
 * - 3 Moderate
 * - 4 Poor
 * - 5 Unhealthy
 */
uint8_t ens160GetAQI();

/**
 * @brief Get the Total Volatile Organic Compounds
 * @return uint16_t The TVOC (range: 0–65000, unit: ppb)
 */
uint16_t ens160GetTVOC();

/**
 * @brief Get CO2 equivalent concentration calculated according to the detected data of VOCs and hydrogen (eCO2 – Equivalent CO2)
 *
 * @return uint16_t The eCO2 (range: 400–65000 unit: ppm):
 * - Excellent(400 - 600)
 * - Good(600 - 800)
 * - Moderate(800 - 1000)
 * - Poor(1000 - 1500)
 * - Unhealthy(> 1500)
 */
uint16_t ens160GetECO2();

#endif
