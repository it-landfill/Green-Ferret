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

#ifndef LOGGERLIB_HPP
#define LOGGERLIB_HPP
#include <Arduino.h>
void logInfo(const char *module, const char *message);
void logInfo(const char *module, const char *message, const char* value);
void logInfo(const char *module, const char *message, String value);
void logInfo(const char *module, const char *message, int value);
void logInfo(const char *module, const char *message, float value);
void logDebug(const char *module, const char *message);
void logDebug(const char *module, const char *message, const char* value);
void logDebug(const char *module, const char *message, String value);
void logDebug(const char *module, const char *message, int value);
void logDebug(const char *module, const char *message, float value);

// All the functions below also have a send flag that, if set to true, sends the message to the MQTT broker
void logError(const char *module, const char *message, bool send = true);
void logError(const char *module, const char *message, const char* value, bool send = true);
void logError(const char *module, const char *message, String value, bool send = true);
void logError(const char *module, const char *message, int value, bool send = true);
void logError(const char *module, const char *message, float value, bool send = true);
void logWarning(const char *module, const char *message, bool send = false);
void logWarning(const char *module, const char *message, const char* value, bool send = false);
void logWarning(const char *module, const char *message, String value, bool send = false);
void logWarning(const char *module, const char *message, int value, bool send = false);
void logWarning(const char *module, const char *message, float value, bool send = false);
#endif