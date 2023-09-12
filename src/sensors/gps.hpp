#ifndef GPS_HPP
#define GPS_HPP

// #define DISABLE_GPS
// #define GPS_DEBUG

#include "../utilities/dataGPSStruct.hpp"

/**
 * @brief Setup the GPS module
 *
 * @return true If the setup was successful
 * @return false If the setup failed
 */
bool gpsSetup();

/**
 * @brief Get the Location object
 *
 * @return GpsPoint or NULL if no point is available
 */
GpsPoint gpsGetLocation();

/**
 * @brief Get the Last Point object
 *
 * @return GpsPoint or NULL if no point is available
 */
GpsPoint gpsGetLastPoint();

/**
 * @brief Update the GPS point
 *
 */
void gpsUpdateGPSPoint();

/**
 * @brief Loop function for the GPS module
 *
 * Parse NMEA sentences and update the GPS point
 *
 */
void gpsLoop();

/**
 * @brief Wait for the GPS to align
 *
 */
void gpsWaitForAlignment();
#endif
