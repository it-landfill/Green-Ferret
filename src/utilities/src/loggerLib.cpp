/**
 * @file timeLib.cpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../loggerLib.hpp"
#include "../stringConverter.hpp"
#include "../../connections/MQTT.hpp"

#include <Arduino.h>

#define INFO "I"
#define ERROR "E"
#define WARNING "W"
#define DEBUG "D"

// Buffer to convert numbers to string
char tmpBuff[20];

void logMessage(const char* level, const char* module, const char *message) {
	Serial.printf("[%s] [%s] %s\n", level, module, message);
}

void logMessage(const char* level, const char* module, const char *message, const char *value) {
	Serial.printf("[%s] [%s] %s %s\n", level, module, message, value);
}

void logMessage(const char* level, const char* module, const char *message, int value) {
	Serial.printf("[%s] [%s] %s %d\n", level, module, message, value);
}

void logMessage(const char* level, const char* module, const char *message, float value) {
	Serial.printf("[%s] [%s] %s %.2f\n", level, module, message, value);
}

void logInfo(const char *module, const char *message) {
	logMessage(INFO, module, message);
}

void logInfo(const char *module, const char *message, const char* value) {
	logMessage(INFO, module, message, value);
}

void logInfo(const char *module, const char *message, String value) {
	logMessage(INFO, module, message, value.c_str());
}

void logInfo(const char *module, const char *message, int value) {
	logMessage(INFO, module, message, value);
}

void logInfo(const char *module, const char *message, float value) {
	logMessage(INFO, module, message, value);
}

void logError(const char *module, const char *message){
	logMessage(ERROR, module, message);
	if (mqttGetStatus()) {
		mqttSendError(message);
	}
}

void logError(const char *module, const char *message, const char* value) {
	logMessage(ERROR, module, message, value);
	if (mqttGetStatus()) {
		char *tmp = (char*) malloc((strlen(message) + strlen(value) + 2)*sizeof(char));
		sprintf(tmp, "%s %s\0", message, value);
		mqttSendError(tmp);
	}
}

void logError(const char *module, const char *message, String value) {
	logMessage(ERROR, module, message, value.c_str());
	if (mqttGetStatus()) {
		char *tmp = (char*) malloc((strlen(message) + value.length() + 2)*sizeof(char));
		sprintf(tmp, "%s %s\0", message, value.c_str());
		mqttSendError(tmp);
	}
}

void logError(const char *module, const char *message, int value) {
	logMessage(ERROR, module, message, value);
	if (mqttGetStatus()) {
		sprintf(tmpBuff, "%d\0", value);
		char *tmp = (char*) malloc((strlen(message) + strlen(tmpBuff) + 2)*sizeof(char));
		sprintf(tmp, "%s %s\0", message, tmpBuff);
		mqttSendError(tmp);
	}
}

void logError(const char *module, const char *message, float value) {
	logMessage(ERROR, module, message, value);
	if (mqttGetStatus()) {
		sprintf(tmpBuff, "%.2f\0", value);
		char *tmp = (char*) malloc((strlen(message) + strlen(tmpBuff) + 2)*sizeof(char));
		sprintf(tmp, "%s %s\0", message, tmpBuff);
		mqttSendError(tmp);
	}
}

void logWarning(const char *module, const char *message){
	logMessage(WARNING, module, message);
	if (mqttGetStatus()) {
		mqttSendWarning(message);
	}
}

void logWarning(const char *module, const char *message, const char* value) {
	logMessage(WARNING, module, message, value);
	if (mqttGetStatus()) {
		char *tmp = (char*) malloc((strlen(message) + strlen(value) + 2)*sizeof(char));
		sprintf(tmp, "%s %s\0", message, value);
		mqttSendWarning(tmp);
	}
}

void logWarning(const char *module, const char *message, String value) {
	logMessage(WARNING, module, message, value.c_str());
	if (mqttGetStatus()) {
		char *tmp = (char*) malloc((strlen(message) + value.length() + 2)*sizeof(char));
		sprintf(tmp, "%s %s\0", message, value.c_str());
		mqttSendWarning(tmp);
	}
}

void logWarning(const char *module, const char *message, int value) {
	logMessage(WARNING, module, message, value);
	if (mqttGetStatus()) {
		sprintf(tmpBuff, "%.2f\0", value);
		char *tmp = (char*) malloc((strlen(message) + strlen(tmpBuff) + 2)*sizeof(char));
		sprintf(tmp, "%s %s\0", message, tmpBuff);
		mqttSendWarning(tmp);
	}
}

void logWarning(const char *module, const char *message, float value) {
	logMessage(WARNING, module, message, value);
	if (mqttGetStatus()) {
		sprintf(tmpBuff, "%.2f\0", value);
		char *tmp = (char*) malloc((strlen(message) + strlen(tmpBuff) + 2)*sizeof(char));
		sprintf(tmp, "%s %s\0", message, tmpBuff);
		mqttSendWarning(tmp);
	}
}

void logDebug(const char *module, const char *message){
	logMessage(DEBUG, module, message);
}

void logDebug(const char *module, const char *message, const char* value) {
	logMessage(DEBUG, module, message, value);
}

void logDebug(const char *module, const char *message, String value) {
	logMessage(DEBUG, module, message, value.c_str());
}

void logDebug(const char *module, const char *message, int value) {
	logMessage(DEBUG, module, message, value);
}

void logDebug(const char *module, const char *message, float value) {
	logMessage(DEBUG, module, message, value);
}