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
// https://github.com/tzapu/WiFiManager
#include <WiFiManager.h>	

#include "../WiFi.hpp"
#include "../../utilities/loggerLib.hpp"
#include "../../utilities/boardInfo.hpp"

#define MODULE_NAME "WiFi"
#define AP_PW "password"

// WiFiManager instance
WiFiManager wifiManager;
// Connection settings reference
ConnectionSettings *connectionSettingsRef;

// WiFiManager parameters
WiFiManagerParameter custom_mqtt_broker("mqttServer", "mqtt server", "", 40);
WiFiManagerParameter custom_mqtt_port("mqttPort", "mqtt port", "1883", 6);
WiFiManagerParameter custom_mqtt_username("mqttUsername", "mqtt username", "", 40);
WiFiManagerParameter custom_mqtt_password("mqttPassword", "mqtt password", "", 40);
WiFiManagerParameter custom_http_host("httpHost", "http host", "", 40);
// NOTE: This HAS to be an IP address, not a hostname. DO NOT specify the port.
WiFiManagerParameter custom_coap_host("coapHost", "coap host", "", 40);

/**
 * @brief Callback that gets called after closing configportal. It saves the new settings.
 * 
 */
void saveConfigCallback () {
	logInfo(MODULE_NAME, "Saving new settings");
	connectionSettingsRef->mqttBroker = custom_mqtt_broker.getValue();
	connectionSettingsRef->mqttPort = atoi(custom_mqtt_port.getValue());
	connectionSettingsRef->mqttUsername = custom_mqtt_username.getValue();
	connectionSettingsRef->mqttPassword = custom_mqtt_password.getValue();
	connectionSettingsRef->httpHost = custom_http_host.getValue();
	connectionSettingsRef->coapHost = custom_coap_host.getValue();
	connectionSettingsSave();
}

void wifiInit(ConnectionSettings *connectionSettings) {
	logInfo(MODULE_NAME, "Initializing WiFi");
	connectionSettingsRef = connectionSettings;
	// Set callback that gets called after closing configportal.
	wifiManager.setSaveConfigCallback(saveConfigCallback);
	// Set WiFiManager parameters.
	wifiManager.addParameter(&custom_mqtt_broker);
	wifiManager.addParameter(&custom_mqtt_port);
	wifiManager.addParameter(&custom_mqtt_username);
	wifiManager.addParameter(&custom_mqtt_password);
	wifiManager.addParameter(&custom_http_host);
	wifiManager.addParameter(&custom_coap_host);
	custom_mqtt_broker.setValue(connectionSettings->mqttBroker.c_str(), 40);
	custom_mqtt_port.setValue(String(connectionSettings->mqttPort).c_str(), 10);
	custom_mqtt_username.setValue(connectionSettings->mqttUsername.c_str(), 40);
	custom_mqtt_password.setValue(connectionSettings->mqttPassword.c_str(), 40);
	custom_http_host.setValue(connectionSettings->httpHost.c_str(), 40);
	custom_coap_host.setValue(connectionSettings->coapHost.c_str(), 40);
	// Set dark theme for WiFiManager.
	wifiManager.setClass("invert");
	// Auto timeout after 5 minutes (300 seconds).
	wifiManager.setConfigPortalTimeout(300);
}

void wifiSetup(){
	logInfo(MODULE_NAME, "Connecting to WiFi");
	// If the connection fails 5 times, force WiFiManager.
	if (connectionSettingsRef->connFailures > 5) {
		logError(MODULE_NAME, "Board has failed to connect or has been rebooted 5 times. Forcing WiFiManager.");
		// Reset connection failures counter and save the settings.
		connectionSettingsRef->connFailures = 0;
		connectionSettingsSave();
		// Start WiFiManager. If it fails, reboot.
		if (!wifiManager.startConfigPortal(getEsp32ID(), AP_PW)) {
			logError(MODULE_NAME, "Failed to connect and hit timeout. Restarting...");
			delay(3000);
			ESP.restart();
		}
	}
	logInfo(MODULE_NAME, "Connecting to WiFi in 2 seconds");
	delay(2000);
	// Connect to WiFi.
	if (!wifiManager.autoConnect(getEsp32ID(), AP_PW)) {
		// Should increase connection failures here but since I do it already in main, can skip it.
		connectionSettingsSave();
		logError(MODULE_NAME, "Failed to connect and hit timeout. Restarting...");
		delay(3000);
		ESP.restart();
	} else connectionSettingsRef->connFailures = 0;
	connectionSettingsSave();

	logInfo(MODULE_NAME, "WiFi connected. IP address:", WiFi.localIP().toString().c_str());
}

bool wifiStatus(bool reconnect) {
	bool status = WiFi.status() == WL_CONNECTED;
	if (WiFi.status() == WL_CONNECTED && reconnect) wifiSetup();
	return status;
}