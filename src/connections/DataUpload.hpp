/**
 * @file DataUpload.hpp
 * @author Alessandro Benetton (aleben98@gmail.com) @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DATAUPLOAD_HPP
#define DATAUPLOAD_HPP

#include "../memory/settings.hpp"

/**
 * @brief Initialize the data uploader.
 * 
 * @param Settings* Pointer to the settings struct
 */
 
void dataUploadSetup(Settings* settings, ConnectionSettings *connSettings);

/**
 * @brief Publish a message using the selected protocol.
 * 
 * @param payload Message to publish
 * @return true Message published
 * @return false Publish failed
 */
bool publishSensorData(char *payload);

/**
 * @brief Loop function for the data uploader, this is needed by some protocols.
 * 
 * @return true 
 * @return false 
 */
bool dataUploadLoop();

#endif