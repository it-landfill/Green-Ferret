/**
 * @file HTTP.cpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "../HTTP.hpp"

#include "../../utilities/loggerLib.hpp"
#include "../../utilities/boardInfo.hpp"

#define MODULE_NAME "HTTP"

const char *httpAddress = NULL;
ConnectionSettings *connSettingsRef2 = NULL;

void httpSetServerAddress() {

	// HTTP Server configuration
	const char serverEndpoint[] = "telemetry";
	const char group[] = "mobile-sensors";

	if (httpAddress != NULL) {
		logError(MODULE_NAME, "Server address already set");
		return;
	}

	// Address length: 6 (http://) + httpHost + endpoint + 1 (separator) + group length + 1 (separator) + clientID length + 1 (null terminator)
	int len = 6 + (connSettingsRef2->httpHost).length() + 1 + strlen(serverEndpoint) + 1 + strlen(group) + 1 + getEsp32IDLen() + 1;
	char *addr = (char*) malloc(len * sizeof(char)); // Allocate memory for the address. This will last until the end of the program so it's ok (probably) to not free it
	sprintf(addr, "http://%s/%s/%s/%s", connSettingsRef2->httpHost.c_str(), serverEndpoint, group, getEsp32ID());
	httpAddress = addr;
	logDebug(MODULE_NAME, "Server address set to", httpAddress);
}

void httpSetup(ConnectionSettings *connSettingsRef) {
	connSettingsRef2 = connSettingsRef;
	httpSetServerAddress();
	logInfo(MODULE_NAME, "HTTP client initialized");
}

bool httpPublishSensorData(char *payload) {
	logDebug(MODULE_NAME, "Publishing sensor data:", payload);
	// Check WiFi connection
	if (WiFi.status() != WL_CONNECTED) {
		logError(MODULE_NAME, "WiFi not connected");
		return false;
	}

	// Create HTTP client
	HTTPClient http;
	http.begin(httpAddress);
	http.addHeader("Content-Type", "application/json");

	int responseCode = http.POST(payload);

	if (responseCode == 200) {
		// Request sent 
		http.end();
		return true;
	} else if (responseCode > 0) {
		// Request sent successfully, but server returned an error
		logWarning(MODULE_NAME, "Server returned error code", responseCode);
		logWarning(MODULE_NAME, "Response:", http.getString());
	} else {
		// Request failed
		logError(MODULE_NAME, "HTTP request failed");
	}

	http.end();	
	return false;
}