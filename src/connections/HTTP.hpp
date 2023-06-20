/**
 * @file HTTP.hpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef HTTP_HPP
#define HTTP_HPP

/**
 * @brief Initialize the HTTP connection.
 * 
 */
void httpSetup();

/**
 * @brief Publish a message to the HTTP server in the sensor data topic.
 * 
 * @param payload Message to publish
 * @return true Message published
 * @return false Publish failed
 */
bool httpPublishSensorData(char *payload);
#endif