#pragma once
/**
 * @file data_store.h
 * @author Christian Prather
 * @brief This is the central mechanism for data storage across the classes used in the program. Its
 * job is to handle the validity and comparison of the data. It also gives all sections of the code
 * a cental place to access data so they dont all need a dependency on the network connection. If
 * this project were to be scaled this would also allow for multiple device types without the need
 * to multiple of these.
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <unordered_map>
#include <vector>

#include "interface/light.h"

/**
 * @brief The data store class responsible to handling all data received from the server and
 * providing it to all subsystems.
 *
 */

// TODO Templatized
class DataStore
{
  public:
    /// Default constructor
    DataStore() = default;

    /**
     * @brief Add the initial devices to data store for later comparisons
     *
     * @param device
     */
    void addInitalDevice(Light device);

    /**
     * @brief Updates the current record with the most recent one from the server
     *
     * @param newRecord
     */
    void updateData(std::unordered_map<std::string, Light> newRecord);

    /**
     * @brief Checks if a device is newly added
     *
     * @param id Device unique id
     * @return true Device is new
     * @return false Device was already added to the server
     */
    bool newEntry(std::string id);

    /**
     * @brief Checks if an existing device has had its values modified
     *
     * @param id Device unique id
     * @param newHash Device unique hash based on member values
     * @return true Device has been modified
     * @return false Device has not been modified
     */
    bool editedEntry(std::string id, std::string newHash);

    /**
     * @brief Get a specific device record
     *
     * @param id Device in question's unique id
     * @return Light
     */
    Light getRecord(std::string id);

    /**
     * @brief Get all the past records from the server (every device)
     *
     * @return std::unordered_map<std::string, Light> unique device id keyed
     */
    std::unordered_map<std::string, Light> getData();

  private:
    // TODO: Templetize this or make one for each device type

    /// Hashmap of devices using device id as key, done for quick access and lookup time
    std::unordered_map<std::string, Light> records;
};