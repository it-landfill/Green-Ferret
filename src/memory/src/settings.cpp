/**
 * @file settings.cpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-08-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
#include <Preferences.h>

#include "../settings.hpp"
#include "../../utilities/loggerLib.hpp"

#define MODULE_NAME "Settings"

Preferences preferences;
Settings *settingsRef1 = NULL;
ConnectionSettings *connSettingsRef = NULL;

void settingsInit(Settings *settings) {
	settingsRef1 = settings;
	if(preferences.begin("settings", true)) {
		settings->protocol = (DataUploadProtocol) preferences.getUInt("protocol", 0);
		settings->trigger = preferences.getUInt("trigger", 0);
		settings->distance = preferences.getUInt("distance", 0);
		settings->time = preferences.getUInt("time", 0);
		preferences.end();
		logInfo(MODULE_NAME, "Settings loaded");
		logDebugf(MODULE_NAME, "Protocol: %d", settings->protocol);
		logDebugf(MODULE_NAME, "Trigger: %d", settings->trigger);
		logDebugf(MODULE_NAME, "Distance: %d", settings->distance);
		logDebugf(MODULE_NAME, "Time: %d", settings->time);
	} else {
		logWarning(MODULE_NAME, "Settings not found");
	}
}

void settingsSave() {
	preferences.begin("settings", false);
	preferences.putUInt("protocol", settingsRef1->protocol);
	preferences.putUInt("trigger", settingsRef1->trigger);
	preferences.putUInt("distance", settingsRef1->distance);
	preferences.putUInt("time", settingsRef1->time);
	preferences.end();
	logInfo(MODULE_NAME, "Settings saved");
	logDebugf(MODULE_NAME, "Protocol: %d", settingsRef1->protocol);
	logDebugf(MODULE_NAME, "Trigger: %d", settingsRef1->trigger);
	logDebugf(MODULE_NAME, "Distance: %d", settingsRef1->distance);
	logDebugf(MODULE_NAME, "Time: %d", settingsRef1->time);
}

void settingsErase() {
	preferences.begin("settings", false);
	preferences.clear();
	preferences.end();
}

void connectionSettingsInit(ConnectionSettings *connSettings) {
	connSettingsRef = connSettings;
	if(preferences.begin("connectionSettings", true)){
		connSettings->WiFiSSID = (char *) preferences.getString("WiFiSSID", "").c_str();
		connSettings->WiFiPassword = (char *) preferences.getString("WiFiPassword", "").c_str();
		connSettings->MQTTBroker = (char *) preferences.getString("MQTTBroker", "").c_str();
		connSettings->MQTTUser = (char *) preferences.getString("MQTTUser", "").c_str();
		connSettings->MQTTPassword = (char *) preferences.getString("MQTTPassword", "").c_str();
		preferences.end();
		logInfo(MODULE_NAME, "Connection settings loaded");
		logDebugf(MODULE_NAME, "WiFi SSID: %s", connSettings->WiFiSSID);
		logDebugf(MODULE_NAME, "WiFi Password: %s", connSettings->WiFiPassword);
		logDebugf(MODULE_NAME, "MQTT Broker: %s", connSettings->MQTTBroker);
		logDebugf(MODULE_NAME, "MQTT User: %s", connSettings->MQTTUser);
		logDebugf(MODULE_NAME, "MQTT Password: %s", connSettings->MQTTPassword);
	} else {
		logWarning(MODULE_NAME, "Connection settings not found");
	}
}

void connectionSettingsSave() {
	preferences.begin("connectionSett", false);
	preferences.putString("WiFiSSID", connSettingsRef->WiFiSSID);
	preferences.putString("WiFiPassword", connSettingsRef->WiFiPassword);
	preferences.putString("MQTTBroker", connSettingsRef->MQTTBroker);
	preferences.putString("MQTTUser", connSettingsRef->MQTTUser);
	preferences.putString("MQTTPassword", connSettingsRef->MQTTPassword);
	preferences.end();
	logInfo(MODULE_NAME, "Connection settings saved");
	logDebugf(MODULE_NAME, "WiFi SSID: %s", connSettingsRef->WiFiSSID);
	logDebugf(MODULE_NAME, "WiFi Password: %s", connSettingsRef->WiFiPassword);
	logDebugf(MODULE_NAME, "MQTT Broker: %s", connSettingsRef->MQTTBroker);
	logDebugf(MODULE_NAME, "MQTT User: %s", connSettingsRef->MQTTUser);
	logDebugf(MODULE_NAME, "MQTT Password: %s", connSettingsRef->MQTTPassword);
}

void connectionSettingsErase() {
	preferences.begin("connectionSett", false);
	preferences.clear();
	preferences.end();
}