/**
 * @file DataUpload.cpp
 * @author Alessandro Benetton (aleben98@gmail.com) 
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Arduino.h>

#include "../DataUpload.hpp"
#include "../../utilities/loggerLib.hpp"
#include "../MQTT.hpp"
#include "../HTTP.hpp"
#include "../COAP.hpp"

#define MODULE_NAME "DataUploader"

Settings* sett;

/**
 * @brief Initialize the data uploader and wait for MQTT config reception.
 * 
 * @param settings The settings struct as defined in settings.hpp
 */
void dataUploadSetup(Settings* settings, ConnectionSettings *connSettings) {
	logDebug(MODULE_NAME, "Initializing data uploader.");

	// Init MQTT client (This is done regardless of the protocol since MQTT is always needed for message reception)
	mqttSetup(settings, connSettings);
	mqttConnect();

	// Initialize HTTP client (but don't connect yet)
	httpSetup(connSettings);

	// Busy waiting to receive config from mqtt
	while (settings->protocol == NONE) {
		mqttLoop();
		logInfo(MODULE_NAME, "Waiting for config from MQTT...\tSleeping for 1 second.");
		delay(1000);
	}
	// Initialize COAP client (but don't connect yet)
	coapSetup(connSettings);

	// Set the protocol
	sett = settings;
}

bool dataUploadLoop() {
	switch (sett->protocol) {
		case MQTT:
			return mqttLoop();
		default:
			return true;
	}
}

bool publishSensorData(char *payload){
	switch (sett->protocol) {
		case MQTT:
			return mqttPublishSensorData(payload);
		case HTTP:
			return httpPublishSensorData(payload);
		case COAP:
			return coapPublishSensorData(payload);
		default:
			return false;
	}
}