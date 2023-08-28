/**
 * @file boardInfo.cpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Arduino.h>

char* ESP32_ID = NULL; // This is a pointer to a string containing the ESP32's unique ID

// Returns the ESP32 chip id as a string of 12 hexadecimal characters.
char * getEsp32ID() {
    // If the chip id has already been converted to a string, return it.
    if (ESP32_ID != NULL) return ESP32_ID;

    // Convert the chip id from an integer to a string and return it.
	uint64_t chipid = ESP.getEfuseMac();
	ESP32_ID = new char[13];
	sprintf(ESP32_ID, "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);
	return ESP32_ID;
}

int getEsp32IDLen() {
	if (ESP32_ID == NULL) getEsp32ID();
	return strlen(ESP32_ID);
}

void printEspHeapInfo() {
	Serial.printf("Heap available: %d/%d\n", ESP.getFreeHeap(), ESP.getHeapSize());
}