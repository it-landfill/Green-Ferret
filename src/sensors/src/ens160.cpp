#include <Arduino.h>

#include "../ens160.hpp"
#include "../../utilities/loggerLib.hpp"

#define MODULE_NAME "ENS160"
#define ENS160_ADDRESS 0x53

// Label used in ens160GetECO2Label()
char label[10];

DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ ENS160_ADDRESS);

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
	logDebug(MODULE_NAME,"Setting power mode:", mode);
	#ifndef DISABLE_ENS160
	ENS160.setPWRMode(mode);
	#endif
}

void setTempAndHum(float ambientTemp, float relativeHumidity) {
	logDebug(MODULE_NAME,"Setting temperature:", ambientTemp);
	logDebug(MODULE_NAME,"Setting humidity:", relativeHumidity);
	#ifndef DISABLE_ENS160
	ENS160.setTempAndHum(ambientTemp, relativeHumidity);
	#endif
}

/*
   https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
   endTransmission() returns:
   0: success.
   1: data too long to fit in transmit buffer.
   2: received NACK on transmit of address.
   3: received NACK on transmit of data.
   4: other error.
   5: timeout
 */
bool ens160Ping() {
	#ifndef DISABLE_ENS160
	Wire.beginTransmission(ENS160_ADDRESS);
	byte error = Wire.endTransmission();
	// No error, nice
	if (error == 0) return true;

	// Well... not so good, but could be worse
	logError(MODULE_NAME, "Sensor not responding, ping returned:", String(error));
	return false;
	#else
	return true;
	#endif
}

uint8_t ens160GetStatus(){
	#ifndef DISABLE_ENS160
	if (!ens160Ping()) return -1;
	uint8_t status = ENS160.getENS160Status();
	return status;
	#else
	return 0;
	#endif
}

uint8_t ens160GetAQI(){
	#ifndef DISABLE_ENS160
	if (!ens160Ping()) return -1;
	return ENS160.getAQI();
	#else
	return 0;
	#endif
}

uint16_t ens160GetTVOC(){
	#ifndef DISABLE_ENS160
	if (!ens160Ping()) return -1.1;
	return ENS160.getTVOC();
	#else
	return 0;
	#endif
}

uint16_t ens160GetECO2(){
	#ifndef DISABLE_ENS160
	if (!ens160Ping()) return -1.1;
	return ENS160.getECO2();
	#else
	return 0;
	#endif
}