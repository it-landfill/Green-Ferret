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


/**
 * @brief List of supported protocols.
 * 
 */
enum DataUploadProtocol {
	NONE,
	MQTT,
	HTTP
};

struct Settings {
    enum DataUploadProtocol protocol;
    int trigger;
    int distance;
    int time;
};

struct ConnectionSettings {
	char *WiFiSSID;
	char *WiFiPassword;
	char *MQTTBroker;
	char *MQTTUser;
	char *MQTTPassword;
};

void settingsInit(Settings *settings);
void settingsSave();
void connectionSettingsInit(ConnectionSettings *connSettings);
void connectionSettingsSave();
#endif