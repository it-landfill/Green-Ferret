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

#ifndef LOGGERLIB_HPP
#define LOGGERLIB_HPP
void logInfo(const char *module, const char *message);
void logError(const char *module, const char *message);
void logWarning(const char *module, const char *message);
void logDebug(const char *module, const char *message);
void logVerbose(const char *module, const char *message);
void logInfof(const char *module, const char *fmt, ...);
void logErrorf(const char *module, const char *fmt, ...);
void logWarningf(const char *module, const char *fmt, ...);
void logDebugf(const char *module, const char *fmt, ...);
void logVerbosef(const char *module, const char *fmt, ...);
#endif