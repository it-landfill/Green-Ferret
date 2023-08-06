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
	if(preferences.begin("connSett", true)){
		connSettings->mqttBroker = preferences.getString("mqttBroker", "");
		connSettings->mqttPort = preferences.getString("mqttPort", "");
		connSettings->mqttUsername = preferences.getString("mqttUsername", "");
		connSettings->mqttPassword = preferences.getString("mqttPassword", "");
		connSettings->connFailures = preferences.getUInt("connFailures", 0);
		preferences.end();
		logInfo(MODULE_NAME, "Connection settings loaded");
		logDebugf(MODULE_NAME, "MQTT Broker: %s", connSettings->mqttBroker);
		logDebugf(MODULE_NAME, "MQTT Port: %s", connSettings->mqttPort);
		logDebugf(MODULE_NAME, "MQTT Username: %s", connSettings->mqttUsername);
		logDebugf(MODULE_NAME, "MQTT Password: %s", connSettings->mqttPassword);
		logDebugf(MODULE_NAME, "Connection failures: %d", connSettings->connFailures);		
	} else {
		logWarning(MODULE_NAME, "Connection settings not found");
	}
}

void connectionSettingsSave() {
	preferences.begin("connSett", false);
	preferences.putString("mqttBroker", connSettingsRef->mqttBroker);
	preferences.putString("mqttPort", connSettingsRef->mqttPort);
	preferences.putString("mqttUsername", connSettingsRef->mqttUsername);
	preferences.putString("mqttPassword", connSettingsRef->mqttPassword);
	preferences.putUInt("connFailures", connSettingsRef->connFailures);
	preferences.end();
	logInfo(MODULE_NAME, "Connection settings saved");
	logDebugf(MODULE_NAME, "MQTT Broker: %s", connSettingsRef->mqttBroker);
	logDebugf(MODULE_NAME, "MQTT Port: %s", connSettingsRef->mqttPort);
	logDebugf(MODULE_NAME, "MQTT Username: %s", connSettingsRef->mqttUsername);
	logDebugf(MODULE_NAME, "MQTT Password: %s", connSettingsRef->mqttPassword);
	logDebugf(MODULE_NAME, "Connection failures: %d", connSettingsRef->connFailures);
}


void connectionSettingsErase() {
	preferences.begin("connSett", false);
	preferences.clear();
	preferences.end();
}