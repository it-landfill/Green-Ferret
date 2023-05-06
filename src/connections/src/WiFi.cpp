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
#include <WiFiMulti.h>

#include "../WiFi.hpp"
#include "../../utilities/loggerLib.hpp"
#include "../secrets.hpp"
WiFiMulti WiFiMulti;

bool wifiSetup(bool rebootOnFail){

	// We start by adding all the known wifi
	for (int i = 0; i < WiFiCredentialNum; i++) {
		WiFiMulti.addAP(WiFiCredentials[i][0], WiFiCredentials[i][1]);
	}

	logInfo("Connecting to WiFi");

	int i = 0;
	while(WiFiMulti.run() != WL_CONNECTED && i++ < 60) {
		delay(500);
	}

	if (WiFi.status() != WL_CONNECTED) {
		logError("WiFi connection failed");
		if (rebootOnFail) {
			logInfo("Rebooting");
			ESP.restart();
		}
		return false;
	}

	logInfof("WiFi connected. IP address: %s", WiFi.localIP().toString().c_str());
	return true;
}

bool wifiStatus(bool reconnect) {
	bool status = WiFi.status() == WL_CONNECTED;
	if (!status && reconnect) {
		status = wifiSetup(false);
	}
	return status;
}