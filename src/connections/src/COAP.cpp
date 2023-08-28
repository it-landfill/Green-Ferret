/**
 * @file COAP.cpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-06-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

#include "../COAP.hpp"

#include "../../utilities/loggerLib.hpp"
#include "../../utilities/boardInfo.hpp"

#define MODULE_NAME "COAP"

IPAddress coapServerAddress;
const char *coapEndpoint = NULL;
const int coapPort = 5683; //TODO: get from settings
ConnectionSettings *connSettingsRef3 = NULL;

// UDP and CoAP class
WiFiUDP Udp;
Coap coap(Udp, 300);

// CoAP client response callback
void coapCallbackResponse(CoapPacket &packet, IPAddress ip, int port) {
	logInfo(MODULE_NAME, "CoAP response received.");
	char p[packet.payloadlen + 1];
	memcpy(p, packet.payload, packet.payloadlen);
	p[packet.payloadlen] = NULL;

	Serial.println(p);


	logInfo(MODULE_NAME, "CoAP response received. Content:", p);
}

void coapSetServerAddress() {

	// Parse ipaddress

	if (coapServerAddress.fromString(connSettingsRef3->coapHost)) { // try to parse into the IPAddress
		Serial.println("Coap ip parsed");
	} else {
		Serial.println("UnParsable COAP IP");
	}

	// COAP Server configuration
	const char group[] = "mobile-sensors";

	if (coapEndpoint != NULL) {
		logError(MODULE_NAME, "Server address already set");
		return;
	}

	// Address length: 9 (telemetry) + group length + 1 (separator) + clientID length + 1 (null terminator)
	int len = 9 + strlen(group) + 1 + getEsp32IDLen() + 1;
	char *addr = (char*) malloc(len * sizeof(char)); // Allocate memory for the address. This will last until the end of the program so it's ok (probably) to not free it
	sprintf(addr, "telemetry/%s/%s", group, getEsp32ID());
	coapEndpoint = addr;
}

void coapSetup(ConnectionSettings *connSettingsRef) {

	connSettingsRef3 = connSettingsRef;

	coapSetServerAddress();
	coap.response(coapCallbackResponse);
	coap.start();

	logInfo(MODULE_NAME, "CoAP client initialized. Endpoint: %s", coapEndpoint);
}

bool coapPublishSensorData(char *payload) {
	logDebug(MODULE_NAME, "Publishing sensor data %s", payload);

	// Check WiFi connection
	if (WiFi.status() != WL_CONNECTED) {
		logError(MODULE_NAME, "WiFi not connected");
		return false;
	}

	coap.put(coapServerAddress, 5683, coapEndpoint, payload);
	return true;
}