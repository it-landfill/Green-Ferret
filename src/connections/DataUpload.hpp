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

/**
 * @brief List of supported protocols.
 * 
 */
enum DataUploadProtocol {
	NONE,
	MQTT,
	HTTP
};

/**
 * @brief Initialize the data uploader.
 * 
 * @param protocol The protocol to use
 */
void dataUploadSetup(enum DataUploadProtocol protocol = MQTT);

/**
 * @brief Publish a message using the selected protocol.
 * 
 * @param payload Message to publish
 * @return true Message published
 * @return false Publish failed
 */
bool publishSensorData(char *payload);

/**
 * @brief Change the protocol used to publish messages.
 * 
 * @param protocol The new protocol to use
 */
void changeDataUploadProtocol(enum DataUploadProtocol protocol);

#endif