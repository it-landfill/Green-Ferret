/**
 * @file WiFi.cpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Arduino.h"
#include "WiFi.h"
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include "../WiFi.hpp"
#include "../../utilities/loggerLib.hpp"
#include "../../utilities/boardInfo.hpp"
#include "../secrets.hpp"

#define MODULE_NAME "WiFi"
#define AP_PW "password"

WiFiManager wifiManager;
ConnectionSettings *connectionSettingsRef;

WiFiManagerParameter custom_mqtt_broker("server", "mqtt server", "", 40);
WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "1883", 6);
WiFiManagerParameter custom_mqtt_username("username", "mqtt username", "", 40);
WiFiManagerParameter custom_mqtt_password("password", "mqtt password", "", 40);

//TODO: Blink led when in config mode

//callback notifying us of the need to save config
void saveConfigCallback () {
	Serial.println("Should save config");
	connectionSettingsRef->mqttBroker = custom_mqtt_broker.getValue();
	connectionSettingsRef->mqttPort = atoi(custom_mqtt_port.getValue());
	connectionSettingsRef->mqttUsername = custom_mqtt_username.getValue();
	connectionSettingsRef->mqttPassword = custom_mqtt_password.getValue();
	connectionSettingsSave();
}

void wifiInit(ConnectionSettings *connectionSettings) {
	logInfo(MODULE_NAME, "Initializing WiFi");
	connectionSettingsRef = connectionSettings;

	wifiManager.setSaveConfigCallback(saveConfigCallback);

	wifiManager.addParameter(&custom_mqtt_broker);
	wifiManager.addParameter(&custom_mqtt_port);
	wifiManager.addParameter(&custom_mqtt_username);
	wifiManager.addParameter(&custom_mqtt_password);
	custom_mqtt_broker.setValue(connectionSettings->mqttBroker.c_str(), connectionSettings->mqttBroker.length());
	custom_mqtt_port.setValue(String(connectionSettings->mqttPort).c_str(), String(connectionSettings->mqttPort).length());
	custom_mqtt_username.setValue(connectionSettings->mqttUsername.c_str(), connectionSettings->mqttUsername.length());
	custom_mqtt_password.setValue(connectionSettings->mqttPassword.c_str(), connectionSettings->mqttPassword.length());

	// set dark theme
	wifiManager.setClass("invert");

	// Auto timeout after 5 minutes
	wifiManager.setConfigPortalTimeout(300);
}

void wifiSetup(){
	logInfo(MODULE_NAME, "Connecting to WiFi");

	if (connectionSettingsRef->connFailures > 5) {
		logError(MODULE_NAME, "Board has failed to connect or has been rebooted 5 times. Forcing WiFiManager");
		connectionSettingsRef->connFailures = 0;
		connectionSettingsSave();
		if (!wifiManager.startConfigPortal(getEsp32ID(), AP_PW)) {
			logError(MODULE_NAME, "failed to connect and hit timeout. Restarting");
			delay(3000);
			//reset and try again, or maybe put it to deep sleep
			ESP.restart();
		}
	}

	logInfo(MODULE_NAME, "Connecting to WiFi in 2 seconds");
	delay(2000);
	if (!wifiManager.autoConnect(getEsp32ID(), AP_PW)) {
		// Should increase connection failures here but since I do it already in main, can skip it.
		connectionSettingsSave();
		logError(MODULE_NAME, "failed to connect and hit timeout. Restarting");
		delay(3000);
		//reset and try again, or maybe put it to deep sleep
		ESP.restart();
	} else connectionSettingsRef->connFailures = 0;

	// Save the settings
	connectionSettingsSave();

	logInfof(MODULE_NAME, "WiFi connected. IP address: %s", WiFi.localIP().toString().c_str());
}

bool wifiStatus(bool reconnect) {
	bool status = WiFi.status() == WL_CONNECTED;
	if (WiFi.status() == WL_CONNECTED && reconnect) {
		wifiSetup();
	}
	return status;
}