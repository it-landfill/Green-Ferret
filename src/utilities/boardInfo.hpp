/**
 * @file boardInfo.hpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef BOARD_INFO_HPP
#define BOARD_INFO_HPP

/**
 * @brief Get the Esp32 ID object
 * 
 * @return char* The ESP32 chip id as a string of 12 hexadecimal characters. DO NOT DELETE THE POINTER!
 */
char * getEsp32ID();

/**
 * @brief Get the Esp32 ID Len object
 * 
 * @return int 
 */
int getEsp32IDLen();

#endif