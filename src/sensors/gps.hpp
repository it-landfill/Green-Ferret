#ifndef GPS_HPP
#define GPS_HPP

#include "../utilities/dataGPSStruct.hpp"

bool gpsSetup();
bool getLocation();
struct gpsPoint getLastPoint();
struct gpsPoint getNewPoint();
void updateGPSPoint();
float getSpeed();
float getMinDistance();

#endif
