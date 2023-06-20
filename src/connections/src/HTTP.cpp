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

const char server[] = "http://pi3aleben:5000";

bool httpPublishSensorData(char *payload) {
	// Check WiFi connection
	if (WiFi.status() != WL_CONNECTED) {
		logError(MODULE_NAME, "WiFi not connected");
		return false;
	}

	// Create HTTP client
	HTTPClient http;
	http.begin(server);
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