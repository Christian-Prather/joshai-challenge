#include "interface/data_store.h"

void DataStore::addInitalDevice(Light device)
{
    // By the time data is here it should have already be processed the way it should be

    // Add the unique device to the master record list
    this->records.insert(std::make_pair(device.id, device));
}

bool DataStore::newEntry(std::string id)
{
    // Check if data is new
    if (this->records.find(id) == this->records.end())
    {
        // No record of this device in the data store, device is newly added
        return true;
    }
    return false;
}

// TODO: Do I want to move the hasher into its own library / files since interface and data store
// are comparing two hashes?
bool DataStore::editedEntry(std::string id, std::string newHash)
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

std::unordered_map<std::string, Light> DataStore::getData() { return records; }

Light DataStore::getRecord(std::string id) { return this->records.at(id); }

void DataStore::updateData(std::unordered_map<std::string, Light> newRecord)
{
    // Update the master record list with newly provided one
    this->records = newRecord;
}