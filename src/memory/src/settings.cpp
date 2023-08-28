/**
 * @file settings.cpp
 * @author Alessandro Benetton (aleben98@gmail.com) 
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
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

// Preferences and connection settings reference.
Preferences preferences;
ConnectionSettings *connSettingsRef = NULL;

void connectionSettingsInit(ConnectionSettings *connSettings) {
	// Set the reference to the connection settings struct.
	connSettingsRef = connSettings;
	// If the connection settings are found inside the preferences, load them.
	if(preferences.begin("connSett", true)){
		// Load the connection settings.
		connSettings->mqttBroker = preferences.getString("mqttBroker", "");
		connSettings->mqttPort = preferences.getUInt("mqttPort", 1883);
		connSettings->mqttUsername = preferences.getString("mqttUsername", "");
		connSettings->mqttPassword = preferences.getString("mqttPassword", "");
		connSettings->httpHost = preferences.getString("httpHost", "");
		connSettings->coapHost = preferences.getString("coapHost", "");
		connSettings->connFailures = preferences.getUInt("connFailures", 0);
		// Close the preferences.
		preferences.end();
		// Log the connection settings.
		logInfo(MODULE_NAME, "Connection settings loaded");
		logDebugf(MODULE_NAME, "MQTT Broker: %s", connSettings->mqttBroker);
		logDebugf(MODULE_NAME, "MQTT Port: %d", connSettings->mqttPort);
		logDebugf(MODULE_NAME, "MQTT Username: %s", connSettings->mqttUsername);
		logDebugf(MODULE_NAME, "MQTT Password: %s", connSettings->mqttPassword);
		logDebugf(MODULE_NAME, "HTTP Host: %s", connSettings->httpHost);
		logDebugf(MODULE_NAME, "COAP Host: %s", connSettings->coapHost);
		logDebugf(MODULE_NAME, "Connection failures: %d", connSettings->connFailures);		
	} else logWarning(MODULE_NAME, "Connection settings not found");
}

void connectionSettingsSave() {
	// Open the preferences in RW-mode (second parameter indicates read/write mode).
	preferences.begin("connSett", false);
	// Save the connection settings.
	preferences.putString("mqttBroker", connSettingsRef->mqttBroker);
	preferences.putUInt("mqttPort", connSettingsRef->mqttPort);
	preferences.putString("mqttUsername", connSettingsRef->mqttUsername);
	preferences.putString("mqttPassword", connSettingsRef->mqttPassword);
	preferences.putString("httpHost", connSettingsRef->httpHost);
	preferences.putString("coapHost", connSettingsRef->coapHost);
	preferences.putUInt("connFailures", connSettingsRef->connFailures);
	// Close the preferences.
	preferences.end();
	// Log the connection settings.
	logInfo(MODULE_NAME, "Connection settings saved");
	logDebugf(MODULE_NAME, "MQTT Broker: %s", connSettingsRef->mqttBroker);
	logDebugf(MODULE_NAME, "MQTT Port: %d", connSettingsRef->mqttPort);
	logDebugf(MODULE_NAME, "MQTT Username: %s", connSettingsRef->mqttUsername);
	logDebugf(MODULE_NAME, "MQTT Password: %s", connSettingsRef->mqttPassword);
	logDebugf(MODULE_NAME, "HTTP Host: %s", connSettingsRef->httpHost);
	logDebugf(MODULE_NAME, "COAP Host: %s", connSettingsRef->coapHost);
	logDebugf(MODULE_NAME, "Connection failures: %d", connSettingsRef->connFailures);
}

void connectionSettingsErase() {
	// Open the preferences in RW-mode (second parameter indicates read/write mode).
	preferences.begin("connSett", false);
	// Erase the connection settings.
	preferences.clear();
	// Close the preferences.
	preferences.end();
}