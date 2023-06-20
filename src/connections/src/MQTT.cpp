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

#include "../MQTT.hpp"

#include "../../utilities/loggerLib.hpp"
#include "../../utilities/boardInfo.hpp"

#define MODULE_NAME "MQTT"

// MQTT Broker configuration
const char broker[] = "pi3aleben";
const char user[] = "IoT";
const char psw[] = "iot2023";
int port = 1883;
const char *clientID = getEsp32ID();
const char *sensorDataTopic;

// MQTT Client
WiFiClient client;
PubSubClient clientMQTT(client);

/**
 * @brief Callback function for MQTT messages.
 * 
 * @param topic The topic of the message
 * @param payload The message
 * @param length The length of the message
 */
void callback(char* topic, byte* payload, unsigned int length) {
	logDebugf(MODULE_NAME, "Message arrived: %f", payload);
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

void mqttSetup() {
	logDebug(MODULE_NAME, "Setting up MQTT");

	setSensorDataTopic();

	// Configure MQTT client parameters
	clientMQTT.setServer(broker, port);
	clientMQTT.setCallback(callback);
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
		if (clientMQTT.connect(clientID, user, psw)) {
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
	clientMQTT.loop();
	return result;
}