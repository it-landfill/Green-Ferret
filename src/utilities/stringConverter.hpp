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

#ifndef STRINGCONVERTER_HPP
#define STRINGCONVERTER_HPP
#include <Arduino.h>
/**
 * Converts a String to a const char array.
 *
 * @param str The String to convert.
 * @return The const char array. MUST be freed after use.
 */
const char * stringToConstCharArr(String str);

/**
 * @brief Converts a variable argument list to a char array.
 * 
 * @param fmt The format string.
 * @param arg The variable argument list.
 * @return char* The char array. MUST be freed after use.
 */
char * varargToCharArr(const char* fmt, va_list arg);
#endif