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

#include <Arduino.h>

#define INFO "I"
#define ERROR "E"
#define WARNING "W"
#define DEBUG "D"
#define VERBOSE "V"

void logMessage(const char* level, const char* module, const char *message) {
	Serial.printf("[%s] [%s] %s\n", level, module, message);
}

void logInfo(const char *module, const char *message) {
	logMessage(INFO, module, message);
}

void logInfof(const char *module, const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);
	logMessage(INFO, module, message);
	free(message);
}

void logError(const char *module, const char *message){
	logMessage(ERROR, module, message);
}

void logErrorf(const char *module, const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);
	logMessage(ERROR, module, message);
	free(message);
}

void logWarning(const char *module, const char *message){
	logMessage(WARNING, module, message);
}

void logWarningf(const char *module, const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);
	logMessage(INFO, module, message);
	free(message);
}

void logDebug(const char *module, const char *message){
	logMessage(DEBUG, module, message);
}

void logDebugf(const char *module, const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);
	logMessage(DEBUG, module, message);
	free(message);
}

void logVerbose(const char *module, const char *message){
	logMessage(VERBOSE, module, message);
}

void logVerbosef(const char *module, const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);
	logMessage(VERBOSE, module, message);
	free(message);
}