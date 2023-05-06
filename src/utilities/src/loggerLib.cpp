/**
 * @file loggerLib.cpp
 * @author Benetton Alessandro (aleben98@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-15
 *
 * @copyright Copyright (c) 2023
 *
 */


#include "../timeLib.hpp"
#include "../loggerLib.hpp"
#include "../stringConverter.hpp"

#include <Arduino.h>

#define INFO "I"
#define ERROR "E"
#define WARNING "W"
#define DEBUG "D"
#define VERBOSE "V"

void logMessagef(const char* fmt, ...){
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);

	Serial.print(message);
	free(message);
}

void logMessage(const char* level, const char *message) {
	// Get timestamp
	char* timestamp = NULL;
	#ifdef TIMELIB_H
	timestamp = getTimestamp();
	logMessagef("[%s] [%s] %s\n", timestamp, level, message);
	// Free memory
	free(timestamp);
	#else
	logMessage("[%s] %s\n", level, message);
	#endif
}

void logInfo(const char *message) {
	logMessage(INFO, message);
}

void logInfof(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);
	logMessage(INFO, message);
	free(message);
}

void logError(const char *message){
	logMessage(ERROR, message);
}

void logErrorf(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);
	logMessage(ERROR, message);
	free(message);
}

void logWarning(const char *message){
	logMessage(WARNING, message);
}

void logWarningf(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);
	logMessage(INFO, message);
	free(message);
}

void logDebug(const char *message){
	logMessage(DEBUG, message);
}

void logDebugf(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);
	logMessage(DEBUG, message);
	free(message);
}

void logVerbose(const char *message){
	logMessage(VERBOSE, message);
}

void logVerbosef(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	char* message = varargToCharArr(fmt, arg);
	va_end(arg);
	logMessage(VERBOSE, message);
	free(message);
}