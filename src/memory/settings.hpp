/**
 * @file settings.hpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
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

#include "../utilities/gpsDistanceUtils.hpp"

/**
 * @brief List of supported protocols for data upload.
 *
 */
enum DataUploadProtocol {
	NONE,
	MQTT,
	HTTP,
	COAP
};

/**
 * @brief Settings struct.
 * @warning  The settings are not saved in EEPROM. Must be received every restart from the server.
 *
 */
struct Settings {
	enum DataUploadProtocol protocol;
	int trigger;
	enum DistanceMethod distanceMethod;
	int distance;
	int time;
};

/**
 * @brief Connection settings struct.
 *
 */
struct ConnectionSettings {
	String mqttBroker;
	uint mqttPort;
	String mqttUsername;
	String mqttPassword;
	String httpHost;
	String coapHost;
};

/**
 * @brief Initialize the connection.
 *
 * @param connSettings  Connection settings struct.
 */
void connectionSettingsInit(ConnectionSettings *connSettings);

/**
 * @brief Save the connection settings.
 *
 */
void connectionSettingsSave();

/**
 * @brief Initialize the reboot count.
 * 
 */
void rebootCountInit();

/**
 * @brief Increase the reboot count by 1.
 * 
 */
void rebootCountIncrease();

/**
 * @brief Decrease the reboot count by 1.
 * 
 */
void rebootCountDecrease();

/**
 * @brief Save the reboot count.
 * 
 * @param val Reboot count.
 */
void rebootCountSet(ushort val);

/**
 * @brief Get the reboot count.
 * 
 * @return ushort Reboot count.
 */
ushort rebootCountGet();

/**
 * @brief Erase the reboot count.
 * 
 */
void rebootCountErase();

#endif