/**
 * @file timeLib.cpp
 * @author Benetton Alessandro (aleben98@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../timeLib.hpp"
#include <arduino.h>

char* getTimestamp() {
	char* timestamp = (char*)malloc(TIMESTAMP_LENGTH+1);

	strncpy(timestamp, "TIME ND", TIMESTAMP_LENGTH+1);

	return timestamp;
}