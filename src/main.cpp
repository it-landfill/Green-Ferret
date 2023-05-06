#include <Arduino.h>
#include "connections/WiFi.hpp"
#include "utilities/loggerLib.hpp"
#include "utilities/timeLib.hpp"
void setup(){
	Serial.begin(115200);
	logInfo("MAIN", "Starting Setup");
	wifiSetup(true);
	setupTime();
	syncTimeServer();
	logInfo("MAIN", "Setup Complete");
}

void loop(){
	delay(100000);
}