#pragma once
/**
 * @file data_store.h
 * @author Christian Prather
 * @brief This is the central mechanism for data storage across the classes used in the program. Its
 * job is to handle the validity and comparison of the data. It also gives all sections of the code
 * a central place to access data so they dont all need a dependency on the network connection. If
 * this project were to be scaled this would also allow for multiple device types. This would mean a
 * data store for each device which is not ideal but for this application is fine. As scale the data
 * should be templatized but the data manager (this) should not.
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

template <typename T> class DataStore
{
  public:
    /// Default constructor
    DataStore() = default;

    /**
     * @brief Add the initial devices to data store for later comparisons
     *
     * @param device
     */
    void addInitalDevice(T device)
    {
        // By the time data is here it should have already be processed the way it should be

        // Add the unique device to the master record list
        this->records.insert(std::make_pair(device.id, device));
    }

    /**
     * @brief Updates the current record with the most recent one from the server
     *
     * @param newRecord
     */
    void updateData(std::unordered_map<std::string, T> newRecord)
    {
        // Update the master record list with newly provided one
        this->records = newRecord;
    }

    /**
     * @brief Checks if a device is newly added
     *
     * @param id Device unique id
     * @return true Device is new
     * @return false Device was already added to the server
     */
    bool newEntry(std::string& id)
    {
        if (this->records.find(id) == this->records.end())
        {
            // No record of this device in the data store, device is newly added
            return true;
        }
        return false;
    }

    /**
     * @brief Checks if an existing device has had its values modified
     *
     * @param id Device unique id
     * @param newHash Device unique hash based on member values
     * @return true Device has been modified
     * @return false Device has not been modified
     */
    bool editedEntry(std::string& id, std::string& newHash)
    {
        // If this is called we have already checked if the device has been seen before
        auto oldDeviceRecord = this->getRecord(id);
        if (newHash != oldDeviceRecord.hashString)
        {
            // Hash strings dont match so devices are not the same values
            return true;
        }
        return false;
    }

    /**
     * @brief Get a specific device record
     *
     * @param id Device in question's unique id
     * @return T device type
     */
    T& getRecord(std::string& id) { return this->records.at(id); }

    /**
     * @brief Get all the past records from the server (every device)
     *
     * @return std::unordered_map<std::string, <Device Type>> unique device id keyed
     */
    std::unordered_map<std::string, T>& getData() { return records; }

  private:
    /// Hashmap of devices using device id as key, done for quick access and lookup time
    std::unordered_map<std::string, T> records;
};