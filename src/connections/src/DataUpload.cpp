/**
 * @file DataUpload.cpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
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

enum DataUploadProtocol dataUploadProtocol = NONE;

void dataUploadSetup(Settings* settings, enum DataUploadProtocol protocol) {
	logDebugf("DataUploader", "Initializing data uploader with protocol %d.", protocol);

	// Init MQTT client (This is done regardless of the protocol since MQTT is always needed for message reception)
	mqttSetup(settings);
	mqttConnect();

	// Initialize HTTP client (but don't connect yet)
	httpSetup();

	// Set the protocol
	dataUploadProtocol = protocol;
}


bool publishSensorData(char *payload){
	switch (dataUploadProtocol) {
		case MQTT:
			return mqttPublishSensorData(payload);
		case HTTP:
			return httpPublishSensorData(payload);
		default:
			return false;
	}
}

void changeDataUploadProtocol(enum DataUploadProtocol protocol) {
	dataUploadProtocol = protocol;
}