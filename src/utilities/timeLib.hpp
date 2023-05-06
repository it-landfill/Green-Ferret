/**
 * @file timeLib.cpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TIMELIB_H
#define TIMELIB_H
#include <Arduino.h>
#include <ezTime.h>

void   setupTime();
void   syncTimeServer();
bool checkTimeServer();
/**
 * @brief Save the timestamp string in the timestamp parameter.
 * Timestamp string is defined as:
 * - "[YY-MM-DD_HH:MM:SS]\0" in normal conditions
 * - "[YY-MM-DD_HH:MM:SS]E\0" where E is a char that indicates the error
 * - "[TIME ND]\0" if time is not set
 * @return char* Array that will contain the timestamp string. MUST be freed after use.
 */
char* getTimestamp();
#endif