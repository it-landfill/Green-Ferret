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
ushort rebootCount;

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
		// Close the preferences.
		preferences.end();
		// Log the connection settings.
		logInfo(MODULE_NAME, "Connection settings loaded");
		logDebug(MODULE_NAME, "MQTT Broker", connSettings->mqttBroker);
		logDebug(MODULE_NAME, "MQTT Port", ((int) connSettings->mqttPort));
		logDebug(MODULE_NAME, "MQTT Username", connSettings->mqttUsername);
		logDebug(MODULE_NAME, "MQTT Password", connSettings->mqttPassword);
		logDebug(MODULE_NAME, "HTTP Host", connSettings->httpHost);
		logDebug(MODULE_NAME, "COAP Host", connSettings->coapHost);
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
	// Close the preferences.
	preferences.end();
	// Log the connection settings.
	logInfo(MODULE_NAME, "Connection settings saved");
	logDebug(MODULE_NAME, "MQTT Broker", connSettingsRef->mqttBroker);
	logDebug(MODULE_NAME, "MQTT Port", ((int) connSettingsRef->mqttPort));
	logDebug(MODULE_NAME, "MQTT Username", connSettingsRef->mqttUsername);
	logDebug(MODULE_NAME, "MQTT Password", connSettingsRef->mqttPassword);
	logDebug(MODULE_NAME, "HTTP Host", connSettingsRef->httpHost);
	logDebug(MODULE_NAME, "COAP Host", connSettingsRef->coapHost);
}

void connectionSettingsErase() {
	// Open the preferences in RW-mode (second parameter indicates read/write mode).
	preferences.begin("connSett", false);
	// Erase the connection settings.
	preferences.clear();
	// Close the preferences.
	preferences.end();
}

void rebootCountInit() {
	if(preferences.begin("rebCount", true)){
		// Loaf the reboot count.
		rebootCount = preferences.getUShort("rebootCount", 0);
		// Close the preferences.
		preferences.end();
		// Log the connection settings.
		logInfo(MODULE_NAME, "Reboot count loaded:", rebootCount);
	} else {
		logWarning(MODULE_NAME, "Reboot count not found, initializing at 0");
		rebootCount = 0;
	};
}

void rebootCountSave() {
// Open the preferences in RW-mode (second parameter indicates read/write mode).
	preferences.begin("rebCount", false);
	// Save the reboot count.
	preferences.putUShort("rebootCount", rebootCount);
	// Close the preferences.
	preferences.end();
}

void rebootCountIncrease() {
	// Increase the reboot count.
	rebootCount++;

	// Save the reboot count.
	rebootCountSave();

	// Log the reboot count.
	logInfo(MODULE_NAME, "Reboot count increased:", rebootCount);
}

void rebootCountDecrease() {
	// Decrease the reboot count.
	rebootCount--;

// Save the reboot count.
	rebootCountSave();

	// Log the reboot count.
	logInfo(MODULE_NAME, "Reboot count decreased:", rebootCount);
}

ushort rebootCountGet() {
	return rebootCount;
}

void rebootCountSet(ushort val) {
	rebootCount = val;

	// Save the reboot count.
	rebootCountSave();
}

void rebootCountErase() {
	// Open the preferences in RW-mode (second parameter indicates read/write mode).
	preferences.begin("rebCount", false);
	// Erase the reboot count.
	preferences.clear();
	// Close the preferences.
	preferences.end();
}