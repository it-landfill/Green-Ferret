/**
 * @file settings.hpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <Arduino.h>

/**
 * @brief List of supported protocols.
 * 
 */
enum DataUploadProtocol {
	NONE,
	MQTT,
	HTTP,
	COAP
};

/**
 * @brief Settings struct. Note that this is not saved in EEPROM. Must be received every restart from the server.
 * 
 */
struct Settings {
    enum DataUploadProtocol protocol;
    int trigger;
    int distance;
    int time;
};

struct ConnectionSettings {
	String mqttBroker;
	uint mqttPort;
	String mqttUsername;
	String mqttPassword;
	uint connFailures;
};

void connectionSettingsInit(ConnectionSettings *connSettings);
void connectionSettingsSave();
#endif