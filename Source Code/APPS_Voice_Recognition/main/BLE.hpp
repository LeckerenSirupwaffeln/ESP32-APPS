/**
 * @file BLE.hpp
 * @author Julian Gaenser (julian.ganser@smail.emt.h-brs.de)
 * @brief BLE module used to interface with Arduino's BLE library.
 * @version 0.1
 * @date 2022-06-25
 * 
 * @copyright Fully open-source
 * 
 */

#pragma once

//Standard library
#include <string>

/**
 * @brief Set up BLE.
 * 
 */
void setup_ble();

/**
 * @brief Attempt to send quality and strength values via BLE.
 * 
 * @param strength_value Input. Amplitude of the base frequency of dynamic speech
 * @param quality_value Input. Quality(deviation) of the base frequency of dynamic speech.
 */
void try_send_voice_values(const std::string& strength_value, const std::string& quality_value);


