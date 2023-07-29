#ifndef GPS_HPP
#define GPS_HPP

#include "../utilities/dataGPSStruct.hpp"

/**
 * @brief Setup the GPS module
 * 
 * @return true If the setup was successful
 * @return false If the setup failed
 */
bool gpsSetup();
/**
 * @brief Get the current location
 * 
 * @return true If the location was successfully retrieved
 * @return false If the location was not successfully retrieved
 */
bool getLocation();
/**
 * @brief Get the last location
 * 
 * @return struct gpsPoint The last location
 */
struct gpsPoint getLastPoint();
/**
 * @brief Get the new location
 * 
 * @return struct gpsPoint The new location
 */
struct gpsPoint getNewPoint();
/**
 * @brief Update the GPS point
 * 
 */
void updateGPSPoint();
/**
 * @brief Get the speed
 * 
 * @return float The speed in km/h
 */
float getSpeed();
/**
 * @brief Get the minimum distance
 * 
 * @return float The minimum distance in km
 */
float getMinDistance();

#endif
