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

#include "../stringConverter.hpp"

const char * stringToConstCharArr(String str) {
	const int strLen = str.length();
	char * charArr = (char*)malloc((strLen + 1) * sizeof(char));
	str.toCharArray(charArr, strLen + 1);
	return charArr;
}

char* varargToCharArr(const char* fmt, va_list arg) {
	// This will be our variable size buffer
	char * temp = NULL;

	va_list copy;

	// Make a copy of the va_list so we can use it a second time
	va_copy(copy, arg);

	// Try to load the string into the buffer
	int len = vsnprintf(temp, 0, fmt, copy);

	// Free the copy
	va_end(copy);

	// Check for error
	if(len < 0) return NULL;

	// If it is, make a new buffer of the correct size
	temp = (char*) malloc(len+1);

	// Check for malloc error
	if(temp == NULL) return NULL;

	// Load the string into the new buffer
	len = vsnprintf(temp, len+1, fmt, arg);


	return temp;
}