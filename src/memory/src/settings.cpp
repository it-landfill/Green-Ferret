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
ConnectionSettings *connSettingsRef = NULL;

void connectionSettingsInit(ConnectionSettings *connSettings) {
	connSettingsRef = connSettings;
	if(preferences.begin("connSett", true)){
		connSettings->mqttBroker = preferences.getString("mqttBroker", "");
		connSettings->mqttPort = preferences.getUInt("mqttPort", 1883);
		connSettings->mqttUsername = preferences.getString("mqttUsername", "");
		connSettings->mqttPassword = preferences.getString("mqttPassword", "");
		connSettings->connFailures = preferences.getUInt("connFailures", 0);
		preferences.end();
		logInfo(MODULE_NAME, "Connection settings loaded");
		logDebugf(MODULE_NAME, "MQTT Broker: %s", connSettings->mqttBroker);
		logDebugf(MODULE_NAME, "MQTT Port: %d", connSettings->mqttPort);
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
	preferences.putUInt("mqttPort", connSettingsRef->mqttPort);
	preferences.putString("mqttUsername", connSettingsRef->mqttUsername);
	preferences.putString("mqttPassword", connSettingsRef->mqttPassword);
	preferences.putUInt("connFailures", connSettingsRef->connFailures);
	preferences.end();
	logInfo(MODULE_NAME, "Connection settings saved");
	logDebugf(MODULE_NAME, "MQTT Broker: %s", connSettingsRef->mqttBroker);
	logDebugf(MODULE_NAME, "MQTT Port: %d", connSettingsRef->mqttPort);
	logDebugf(MODULE_NAME, "MQTT Username: %s", connSettingsRef->mqttUsername);
	logDebugf(MODULE_NAME, "MQTT Password: %s", connSettingsRef->mqttPassword);
	logDebugf(MODULE_NAME, "Connection failures: %d", connSettingsRef->connFailures);
}


void connectionSettingsErase() {
	preferences.begin("connSett", false);
	preferences.clear();
	preferences.end();
}