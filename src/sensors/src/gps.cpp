#include <Arduino.h>
#include "../gps.hpp"

#include "TinyGPS++.h"
#include "../../utilities/loggerLib.hpp"

#define MODULE_NAME "GPS"

// GPS object
TinyGPSPlus gps;

#define GPSBaud 9600
#define GPStimeout 300000 // 5 minutes

// Initialize the last point and new point to 0
struct gpsPoint lastPoint = { 0, 0, 0 };
struct gpsPoint newPoint = { 0, 0, 0 };

unsigned long lastTime = 0;
unsigned long lastWarning = 0;

void displayInfo() {
	Serial.print(F("Location: "));
	if (gps.location.isValid()) {
		Serial.print(gps.location.lat(), 6);
		Serial.print(F(","));
		Serial.print(gps.location.lng(), 6);
	} else Serial.print(F("INVALID"));

	Serial.print(F("  Date/Time: "));
	if (gps.date.isValid()) {
		Serial.print(gps.date.month());
		Serial.print(F("/"));
		Serial.print(gps.date.day());
		Serial.print(F("/"));
		Serial.print(gps.date.year());
	} else Serial.print(F("INVALID"));

	Serial.print(F(" "));
	if (gps.time.isValid()) {
		if (gps.time.hour() < 10) Serial.print(F("0"));
		Serial.print(gps.time.hour());
		Serial.print(F(":"));
		if (gps.time.minute() < 10) Serial.print(F("0"));
		Serial.print(gps.time.minute());
		Serial.print(F(":"));
		if (gps.time.second() < 10) Serial.print(F("0"));
		Serial.print(gps.time.second());
		Serial.print(F("."));
		if (gps.time.centisecond() < 10) Serial.print(F("0"));
		Serial.print(gps.time.centisecond());
	} else Serial.print(F("INVALID"));

	Serial.println();
}


// GPS setup
bool gpsSetup() {
	#ifndef DISABLE_GPS
	logDebug(MODULE_NAME, "Begin setup");

	Serial2.begin(GPSBaud);



	logDebug(MODULE_NAME, "GPS initialized correctly");

	logDebug(MODULE_NAME, "End setup");
	#else
	logWarning(MODULE_NAME, "GPS disabled");
	#endif
	return true;
}

void gpsLoop() {
	#ifndef DISABLE_GPS
	// This sketch displays information every time a new sentence is correctly encoded.
	while (Serial2.available() > 0) {
		gps.encode(Serial2.read());
		lastTime = millis();
		lastWarning = millis();
	}
	#ifdef GPS_DEBUG
	displayInfo();
	#endif

	// Non devo gestire rollover di millis() grazie al fatto che sono unsigned long
	if (millis() - lastWarning > GPStimeout) {
		logWarning(MODULE_NAME, "No GPS data received. Last message was (minutes ago): ", (int)((millis() - lastTime) / 60000));
		displayInfo();
		lastWarning = millis();
	}
	#endif
}

void gpsWaitForAlignment() {
	logInfo(MODULE_NAME, "Waiting for GPS alignment");
	#ifndef DISABLE_GPS
	while (gpsGetLocation() == NULL) {
		gpsLoop();
		#ifndef GPS_DEBUG
		Serial.print(".");
		#endif
		delay(1000);
	}
	#endif
	logInfo(MODULE_NAME, "GPS aligned");
}


// Get location
GpsPoint gpsGetLocation() {
	#ifndef DISABLE_GPS
	// If there is a new location
	if (gps.location.isValid()) {
		// If the new location is different from the last one
		if (gps.location.lat() != lastPoint.lat || gps.location.lng() != lastPoint.lon) {
			// Set the new point
			newPoint.lat = gps.location.lat();
			newPoint.lon = gps.location.lng();
			newPoint.timestamp = gps.time.value();
			return &newPoint;
		}
	}
	#endif
	return &newPoint;
}

// Return the last gps point
GpsPoint gpsGetLastPoint() {
	#ifndef DISABLE_GPS
	if (gps.location.isValid()) return &lastPoint;
	else return NULL;
	#else
	return &lastPoint;
	#endif
}


// Update the last point with the new one.
void gpsUpdateGPSPoint() {
	lastPoint.lat = newPoint.lat;
	lastPoint.lon = newPoint.lon;
	lastPoint.timestamp = newPoint.timestamp;
}
