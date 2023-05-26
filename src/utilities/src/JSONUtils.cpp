/**
 * @file JSONUtils.cpp
 * @brief JSONUtils is used to serialize and deserialize data to and from JSON
 * @version 0.1
 * @date 2023-05-25
 *
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../JSONUtils.hpp"

#define MODULE_NAME "JSONUtils"
// The JSON buffer size is the maximum number of characters in a JSON object.
// This is the number of characters in the longest JSON object, plus 10.
#define JSON_BUFFER_SIZE 130

char* serializeSensorData(float *temperature, float *humidity, float *pressure, float *latitude, float *longitude, int *aqi, int *tvoc, int *eco2) {
	// JsonDocuments are made to be single use, should NOT re use them (https://arduinojson.org/v6/how-to/reuse-a-json-document/)
	StaticJsonDocument<JSON_BUFFER_SIZE> doc;

	// Add the data to the json document if it is not null
	if (temperature != NULL) doc["tem"] = *temperature;
	if (humidity != NULL) doc["hum"] = *humidity;
	if (pressure != NULL) doc["pre"] = *pressure;
	if (latitude != NULL && longitude != NULL) {
		doc["lat"] = *latitude;
		doc["lon"] = *longitude;
	}
	if (aqi != NULL) doc["aqi"] = *aqi;
	if (tvoc != NULL) doc["tvo"] = *tvoc;
	if (eco2 != NULL) doc["eco"] = *eco2;

	// Allocate the memory for the output
	const int size = doc.memoryUsage()+1;
	char *output = (char *)malloc(size);

	// Serialize the json document
	serializeJson(doc, output, JSON_BUFFER_SIZE);

	return output;
}