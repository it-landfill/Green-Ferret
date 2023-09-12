#ifndef ATH20_HPP
#define AHT20_HPP

// #define DISABLE_AHT20

/**
 * @brief Initializes sensor communication
 * 
 * @return true If sensor initialization succeded
 * @return false If sensor initialization failed
 */
bool aht20Setup();

/**
 * @brief Returns the last read temperature
 * 
 * @return float The temperature in °C
 */
float aht20GetTemperature();

/**
 * @brief Returns the last read hummidity
 * 
 * @return float The hummidity in % RH
 */
float aht20GetHumidity();
#endif