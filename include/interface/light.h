#pragma once
/**
 * @file light.h
 * @author Christian Prather
 * @brief This file contains a class structure for light devices and their relative data
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>

/**
 * @brief Light specific device
 * @note This could be abstracted to inherit from a device parent class
 *
 */
class Light
{
  public:
    /**
     * @brief Construct a new Light object
     *
     * @param brightness int 0-255 brightness from server
     * @param id string unique Id of device
     * @param name string name given to device by user
     * @param on Bool true = on
     * @param room string what room the device is in
     */
    Light(uint8_t brightness, std::string id, std::string name, bool on, std::string room);

    // TODO: Add getters and make these private
    uint8_t brightness;
    std::string id;
    std::string name;
    bool on;
    std::string room;
    std::hash<std::string> hasher;
    std::string hashString;

    // Helpers

    /**
     * @brief Generates a hash based on all the member variable values at time of contraction. Used
     * to compare if elements have changed efficiently later on in the program
     *
     */
    void generateHash();

    /**
     * @brief Map the 0-255 pwm brightness values to a percent 0-100
     *
     * @param value Input value (0-255)
     * @return int output percentage (0-100)
     */
    int convertBrightness(int value);

    /**
     * @brief Handle editing / conditioning the device data to match what is usable. For light
     * device this is only the brightness level.
     *
     */
    void parseData();
};

// Operator overload for ==
inline bool operator==(const Light& lhs, const Light& rhs)
{
    if (lhs.hashString == rhs.hashString)
    {
        return true;
    }
    return false;
}

inline bool operator!=(const Light& lhs, const Light& rhs) { return !(lhs == rhs); }