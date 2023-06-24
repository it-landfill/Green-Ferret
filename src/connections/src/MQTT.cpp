/**
 * @file MQTT.cpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-21
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "../MQTT.hpp"

#include "../../utilities/loggerLib.hpp"
#include "../../utilities/boardInfo.hpp"

#define MODULE_NAME "MQTT"

// MQTT Broker configuration
const char *clientID = getEsp32ID();
const char *sensorDataTopic;

// MQTT Client
WiFiClient client;
PubSubClient clientMQTT(client);
Settings *settingsRef = NULL;
ConnectionSettings *connSettingsRef1 = NULL;

/**
 * @brief Parse received config and update settings.
 *
 * @param json
 */
void parseMessage(char* json) {
	StaticJsonDocument<64> doc;
	DeserializationError error = deserializeJson(doc, json);

	if (error) {
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.f_str());
		return;
	}

	int protocol = doc["protocol"]; // 1
	int trigger = doc["trigger"]; // 1
	int distance = doc["distance"]; // 5
	int time = doc["time"]; // 10

	settingsRef->protocol = static_cast<DataUploadProtocol>(protocol);
	settingsRef->trigger = trigger;
	settingsRef->distance = distance;
	settingsRef->time = time;

	logDebug(MODULE_NAME, "Settings received:");
	logDebugf(MODULE_NAME, "Protocol: %d", protocol);
	logDebugf(MODULE_NAME, "Protocol enum: %d", static_cast<DataUploadProtocol>(protocol));
	logDebugf(MODULE_NAME, "Trigger: %d", trigger);
	logDebugf(MODULE_NAME, "Distance: %d", distance);
	logDebugf(MODULE_NAME, "Time: %d", time);
}

/**
 * @brief Callback function for MQTT messages.
 *
 * @param topic The topic of the message
 * @param payload The message
 * @param length The length of the message
 */
void callback(char* topic, byte* payload, unsigned int length) {
	// Config topic should be CFG/<esp32ID>/Config (CFG/A49879286F24/Config)

	char * pch;
	// Strtok splits a string into tokens with the specified divider. https://cplusplus.com/reference/cstring/strtok/
	pch = strtok (topic,"/");
	if (pch != NULL && strcmp(pch, "CFG") != 0) {
		logWarningf(MODULE_NAME, "Received message on topic %s, but expected CFG", topic);
		return;
	}

	// Calling strtok with NULL returns the next token in the string. The second token should be ESP ID and we can ignore it since we receive only what we subscribed for.
	pch = strtok (NULL, "/");
	if (pch == NULL) {
		logWarningf(MODULE_NAME, "Received message on topic %s, but expected ESP ID", topic);
		return;
	}

	// Third token is the message type. We expect Config.
	pch = strtok (NULL, "/");
	if (pch != NULL && strcmp(pch, "Config") == 0) {
		char* p = (char*)malloc(length + 1);
		memcpy(p, payload, length);
		parseMessage(p);
		free(p);
	} else {
		logWarningf(MODULE_NAME, "Received message on topic %s, but expected Config", topic);
		return;
	}
}

/**
 * @brief Generate the configuration topic.
 *
 * @return char* The configuration topic
 */
char* genConfigTopic() {
	char *topic = new char[30];
	sprintf(topic, "CFG/%s/Config", getEsp32ID());
	return topic;
}

/**
 * @brief Set the Sensor Data Topic.
 *
 */
void setSensorDataTopic() {
	if (sensorDataTopic != NULL) return;

	char *topic = new char[30];
	sprintf(topic, "mobile-sensors/%sa", getEsp32ID());
	sensorDataTopic = topic;
}

void mqttSetup(Settings *settings, ConnectionSettings *connSettingsRef) {
	logDebug(MODULE_NAME, "Setting up MQTT");

	settingsRef = settings;
	connSettingsRef1 = connSettingsRef;

	setSensorDataTopic();

	// Configure MQTT client parameters
	clientMQTT.setServer(connSettingsRef1->mqttBroker.c_str(), connSettingsRef1->mqttPort);
	clientMQTT.setCallback(callback);
	logInfo(MODULE_NAME, "MQTT setup completed");
}

/**
 * @brief Subscribe to the given topics.
 *
 * @param topics List of topics to subscribe to
 */
void mqttSubscribe(char* topics[]) {
	for (int i = 0; i < sizeof(topics) / sizeof(topics[0]); i++) {
		logDebugf(MODULE_NAME, "Subscribing to topic: %s", topics[i]);
		bool res = clientMQTT.subscribe(topics[i]);
		if (res) logDebugf(MODULE_NAME, "Subscribed to topic %s", topics[i]);
		else logDebugf(MODULE_NAME, "Subscription failed to topic %s", topics[i]);
	}

}

bool mqttConnect() {
	if (!clientMQTT.connected()) {
		logDebug(MODULE_NAME, "Connecting to MQTT broker");

		// Connect to MQTT broker
		if (connSettingsRef1 == NULL) {
			logError(MODULE_NAME, "Connection settings not set");
			return false;
		}

		if (clientMQTT.connect(clientID, connSettingsRef1->mqttUsername.c_str(), connSettingsRef1->mqttPassword.c_str())) {
			logDebug(MODULE_NAME, "Connected to MQTT broker");

			char *topics[] = {genConfigTopic()};
			mqttSubscribe(topics);
			return true;
		}
		else {
			logError(MODULE_NAME, "MQTT Broker not available");
			return false;
		}
	}
	return true;
}

bool mqttPublishSensorData(char *payload) {
	// Connect to MQTT broker if not connected
	if (!clientMQTT.connected()) mqttConnect();

	// If still not connected, return false
	if (!clientMQTT.connected()) {
		logError(MODULE_NAME, "MQTT Broker not available");
		return false;
	}

	bool result = clientMQTT.publish(sensorDataTopic, payload);
	return result;
}

bool mqttLoop() {
	if (!clientMQTT.connected()) mqttConnect();
	clientMQTT.loop();
	return clientMQTT.connected();
}