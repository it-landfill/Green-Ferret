#include <Arduino.h>
#include "../gps.hpp"

#include "TinyGPS++.h"
#include "../../utilities/loggerLib.hpp"

#define MODULE_NAME "GPS"

// A sample NMEA stream.
// TODO: Search the meaning of the fields
const char* gpsStream =
"$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
"$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
"$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
"$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
"$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
"$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

// GPS object
TinyGPSPlus gps;

// Initialize the last point and new point to 0
struct gpsPoint lastPoint = { 0, 0, 0 };
struct gpsPoint newPoint = { 0, 0, 0 };


// Set the minimum distance to 0.1 km (100 m) to publish
#define minDistance 0.1

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
	logDebug(MODULE_NAME, "Begin setup");

	while (*gpsStream)
		if (gps.encode(*gpsStream++))
			displayInfo();

	logDebug(MODULE_NAME, "GPS initialized correctly");

	logDebug(MODULE_NAME, "End setup");
	return true;
}


// Get location
bool getLocation() {
	// If there is a new location
	if (gps.location.isValid()) {
		// If the new location is different from the last one
		if (gps.location.lat() != lastPoint.lat || gps.location.lng() != lastPoint.lon) {
			// Set the new point
			newPoint.lat = gps.location.lat();
			newPoint.lon = gps.location.lng();
			newPoint.timestamp = gps.time.value();
			return true;
		}
	}
	return false;
}

// Return the last gps point
struct gpsPoint getLastPoint() {
	return lastPoint;
}

// Return the new gps point
struct gpsPoint getNewPoint() {
	getLocation();
	return newPoint;
}

// Update the last point with the new one.
void updateGPSPoint() {
	lastPoint.lat = newPoint.lat;
	lastPoint.lon = newPoint.lon;
	lastPoint.timestamp = newPoint.timestamp;
}

// Get minimum distance to publish
float getMinDistance() {
	return minDistance;
}

// Return the speed in km/h
float getSpeed() {
	return gps.speed.kmph();
}
