/**
 * @file WiFi.hpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WIFI_HPP
#define WIFI_HPP

#include "../memory/settings.hpp"

void wifiInit(ConnectionSettings *connectionSettings);

/**
 * @brief Setup WiFi connection.
 *
 * @return true If the connection succedes
 * @return false If the connection fails and rebootOnFail is false
 */
void wifiSetup();

/**
 * @brief Check the WiFi connection status.
 *
 * @param reconnect If true, try to reconnect if the connection is lost. Reconnection calls wifiSetup(false). (Default true)
 * @return true If the connection succedes
 * @return false If the connection fails and rebootOnFail is false
 */
bool wifiStatus(bool reconnect=true);
#endif