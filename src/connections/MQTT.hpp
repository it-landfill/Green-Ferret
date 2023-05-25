/**
 * @file MQTT.hpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-21
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MQTT_HPP
#define MQTT_HPP
void connectMQTT();
void subscribeTopicMQTT(char *topic);
bool publishData(char *payload);
char *getEsp32ID();
char* genConfigChannel();
void setDataChannel();
#endif