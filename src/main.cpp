#include <Arduino.h>
#include "connections/WiFi.hpp"
void setup(){
	Serial.begin(115200);
	wifiSetup(true);
}

void loop(){
	delay(100000);
}