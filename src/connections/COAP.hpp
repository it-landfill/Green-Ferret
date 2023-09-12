/**
 * @file COAP.hpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef COAP_HPP
#define COAP_HPP
#include "../memory/settings.hpp"
/**
 * @brief Initialize the COAP connection.
 * 
 */
void coapSetup(ConnectionSettings *connSettingsRef);

/**
 * @brief Publish a message to the COAP server in the sensor data topic.
 * 
 * @param payload Message to publish
 * @return true Message published
 * @return false Publish failed
 */
bool coapPublishSensorData(char *payload);

#endif