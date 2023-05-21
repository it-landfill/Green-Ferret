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

const char broker[] = "test.mosquitto.org";
int port = 1883;
const char clientID[] = "cresp";
const char *topicPushData = "CFG/Data";

PubSubClient clientMQTT;
WiFiClient client;

void connectMQTT() {
    client = WiFiClient();
    clientMQTT.setClient(client);
    Serial.printf("Connecting to MQTT broker \n");
    clientMQTT.setServer(broker, port);
    if (clientMQTT.connect(clientID)) Serial.printf("Connected to MQTT broker \n");
    else {
        Serial.printf("Connection to MQTT broker failed");
        Serial.printf("Error code: %d \n", clientMQTT.state());
    }
}

void subscribeTopicMQTT(char *topic) {
	Serial.printf("Subscribing to topic %s \n", topic);
    clientMQTT.subscribe(topic);
	Serial.printf("Subscribed to topic %s \n", topic);
}

bool publishData(char *topic, char *payload) {
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