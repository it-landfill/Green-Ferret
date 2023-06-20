#include <Arduino.h>

#include "../ens160.hpp"
#include "../../utilities/loggerLib.hpp"

#define MODULE_NAME "ENS160"

// Label used in ens160GetECO2Label()
char label[10];

DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x53);

bool ens160Setup(float ambientTemp, float ambientHum, uint8_t mode) {
	logDebug(MODULE_NAME,"Begin setup");

	#ifndef DISABLE_ENS160
	// Init the sensor
	int i = 0;
	while (NO_ERR != ENS160.begin() && i++ < 10) {
		logWarning(MODULE_NAME,"Communication with device failed, retrying in 3 seconds");
		delay(3000);
	}

	if (i == 10) {
		logError(MODULE_NAME, "Communication with device failed, please check connection");
		return false;
	} else {
		logDebug(MODULE_NAME,"Begin ok!");
	}

	/**
	 * Set power mode
	 * mode Configurable power mode:
	 *   ENS160_SLEEP_MODE: DEEP SLEEP mode (low power standby)
	 *   ENS160_IDLE_MODE: IDLE mode (low-power)
	 *   ENS160_STANDARD_MODE: STANDARD Gas Sensing Modes
	 */
	ENS160.setPWRMode(mode);

	/**
	 * Users write ambient temperature and relative humidity into ENS160 for
	 * calibration and compensation of the measured gas data. ambientTemp
	 * Compensate the current ambient temperature, float type, unit: C
	 * relativeHumidity Compensate the current ambient temperature, float type,
	 * unit: %rH
	 *
	 */
	ENS160.setTempAndHum(ambientTemp, ambientHum);
	#else
	logWarning(MODULE_NAME, "ENS160 disabled");
	#endif

	logDebug(MODULE_NAME,"End setup");
	return true;
}

void ens160SetPWRMode(uint8_t mode) {
	logVerbosef(MODULE_NAME,"Setting power mode: %d\n", mode);
	#ifndef DISABLE_ENS160
	ENS160.setPWRMode(mode);
	#endif
}

void setTempAndHum(float ambientTemp, float relativeHumidity) {
	logVerbosef(MODULE_NAME,"Setting temperature: %f and humidity: %f\n", ambientTemp, relativeHumidity);
	#ifndef DISABLE_ENS160
	ENS160.setTempAndHum(ambientTemp, relativeHumidity);
	#endif
}

uint8_t ens160GetStatus(){
	#ifndef DISABLE_ENS160
	return ENS160.getENS160Status();
	#else
	return -1;
	#endif
}

uint8_t ens160GetAQI(){
	#ifndef DISABLE_ENS160
	return ENS160.getAQI();
	#else
	return -1;
	#endif
}

uint16_t ens160GetTVOC(){
	#ifndef DISABLE_ENS160
	return ENS160.getTVOC();
	#else
	return -1;
	#endif
}

uint16_t ens160GetECO2(){
	#ifndef DISABLE_ENS160
	return ENS160.getECO2();
	#else
	return -1;
	#endif
}

char* ens160GetECO2Label(){
	#ifndef DISABLE_ENS160
	uint16_t val = ens160GetECO2();

	if (val>1500) strcpy(label, "Unhealthy");
	else if (val>1000) strcpy(label, "Poor");
	else if (val>800) strcpy(label, "Moderate");
	else if (val>600) strcpy(label, "Good");
	else strcpy(label, "Excelent");

	return label; 
	#else
	return "Disabled";
	#endif
}