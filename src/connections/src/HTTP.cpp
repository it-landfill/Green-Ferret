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

void httpSetServerAddress() {

	// HTTP Server configuration
	const char serverBaseAddress[] = "http://pi3aleben:5000/telemetry"; //TODO: Get this from settings
	const char group[] = "mobile-sensors";

	if (httpAddress != NULL) {
		logError(MODULE_NAME, "Server address already set");
		return;
	}

	// Address length: 31 (base) + 1 (separator) + group length + 1 (separator) + clientID length + 1 (null terminator)
	int len = strlen(serverBaseAddress) + 1 + strlen(group) + 1 + getEsp32IDLen() + 1;
	char *addr = (char*) malloc(len * sizeof(char)); // Allocate memory for the address. This will last until the end of the program so it's ok (probably) to not free it
	sprintf(addr, "%s/%s/%s", serverBaseAddress, group, getEsp32ID());
	httpAddress = addr;
}

void httpSetup() {
	httpSetServerAddress();
	logInfo(MODULE_NAME, "HTTP client initialized");
}

bool httpPublishSensorData(char *payload) {
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
		logDebug(MODULE_NAME, "Data upload successful");
		http.end();
		return true;
	} else if (responseCode > 0) {
		// Request sent successfully, but server returned an error
		logWarningf(MODULE_NAME, "Server returned error code %d.\t%s", responseCode, http.getString());
	} else {
		// Request failed
		logError(MODULE_NAME, "HTTP request failed");
	}

	http.end();	
	return false;
}