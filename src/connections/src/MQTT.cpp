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

#include "Arduino.h"
#include "WiFi.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "../../utilities/loggerLib.hpp"

#define MODULE_NAME "MQTT"

const char broker[] = "test.mosquitto.org";
int port = 1883;
const char clientID[] = "cresp";
const char *topicPushData = "CFG/Data";

WiFiClient client;
PubSubClient clientMQTT(client);

void callback(char* topic, byte* payload, unsigned int length) {
	logDebug(MODULE_NAME, "Message arrived");
	StaticJsonDocument<200> doc;
	deserializeJson(doc, payload);
	int communicationMode = doc["CommunicationMode"];
	logDebugf(MODULE_NAME, "Communication mode: %d", communicationMode);
}

void connectMQTT() {
    logDebug(MODULE_NAME, "Connecting to MQTT broker");
	client = WiFiClient();
	clientMQTT.setServer(broker, port);
	clientMQTT.setCallback(callback);
    if (clientMQTT.connect(clientID)) logDebug(MODULE_NAME, "Connected to MQTT broker");
    else logError(MODULE_NAME, "MQTT Broker not available");
}

void subscribeTopicMQTT(char *topic) {
    clientMQTT.subscribe(topic);
	logDebug(MODULE_NAME, "Subscribed to config topic");
}

bool publishData(char *payload) {
    bool connected = clientMQTT.connected();
    if (!connected) connected = clientMQTT.connect(clientID);
    if (connected) {
        bool result = clientMQTT.publish(topicPushData, payload);
        clientMQTT.loop();
        return result;
    }
    else Serial.println(F("MQTT Broker not available"));
    return (false);
}

char *getEsp32ID() {
	uint64_t chipid = ESP.getEfuseMac();
	char *id = new char[13];
	sprintf(id, "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);
	return id;
}

char* genConfigChannel() {
	char *channel = new char[30];
	sprintf(channel, "CFG/%s/Config", getEsp32ID());
	return channel;
}

void setDataChannel() {
	char *channel = new char[30];
	sprintf(channel, "CFG/%s/Data", getEsp32ID());
	topicPushData = channel;
}