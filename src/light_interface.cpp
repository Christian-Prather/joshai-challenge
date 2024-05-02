#include <chrono>
#include <thread>

#include "interface/light_interface.h"

using namespace std::chrono_literals;

const std::string GENERIC_LIGHTS_COMMAND = "/lights";
const int DEVICE_MISSING = 404;

LightsInterface::LightsInterface(std::shared_ptr<ConnectionManager> connectionManager,
                                 std::shared_ptr<DataStore<Light>> dataStore)
{
    // Setup members
    this->connectionManager = connectionManager;
    this->dataStore = dataStore;
}

void LightsInterface::buildInitalDeviceObjects(json light, std::vector<json>& formattedIntialData)
{
    // TODO in situations were the device was already seen by the program and has not changed it
    // would be more efficient to just get the Light object already built in the data store rather
    // then create a new object, this would require a check for device existing in map and if its
    // the same at this level however and that is currently handled in the data store

    // Get device id for reuse
    std::string deviceId = light["id"];

    // Get more details on specific device (note: need to handle delete race condition)
    std::string command = GENERIC_LIGHTS_COMMAND + "/" + deviceId;
    auto detailedLight = this->connectionManager->get(command);
    // Check if light record was still found in the server by the time we wanted to get more
    // info on it.
    // If expression is false the device not found on server anymore, likely deleted between query
    // time
    if (detailedLight.returnCode != DEVICE_MISSING)
    {
        // If here the device is still seen by the server so construct and light specific object.
        // Note this auto creates the hash as well.
        auto device =
            Light(detailedLight.body["brightness"], detailedLight.body["id"],
                  detailedLight.body["name"], detailedLight.body["on"], detailedLight.body["room"]);

        // Add device to master record
        this->dataStore->addInitalDevice(device);
        // Update the brightness value to be percent for printing. (Note this happens
        // automatically when light object was created, just not for pretty printing)
        detailedLight.body["brightness"] = device.brightness;
        // Add light to printing structure
        formattedIntialData.push_back(detailedLight.body);
    }
}

void LightsInterface::getInitalStatus()
{
    // Get data from server
    std::string command = GENERIC_LIGHTS_COMMAND;
    auto serverRespons = this->connectionManager->get(command);

    // Data structure for pretty printing. TODO: look at more efficient way to not reallocate memory
    // for the same data
    std::vector<json> formattedIntialData;

    // Go through each light reported by the server
    for (auto& light : serverRespons.body)
    {
        this->buildInitalDeviceObjects(light, formattedIntialData);
    }

    // TODO run through print class
    json jsonList(formattedIntialData);
    std::cout << jsonList.dump(4) << std::endl;
}

void LightsInterface::run(int hz)
{
    // Setup initial state records
    this->getInitalStatus();

    // Find target ms delay
    int targetDelta = 1000 / hz;

    // Run until user terminates program and get updates on devices
    while (true)
    {
        // TODO much of this is a repeat from the initial list construction but due to slight
        // additional checks is not a direct copy. Ideally this code should not be so wet.
        const auto startTime = std::chrono::high_resolution_clock::now();

        std::string command = GENERIC_LIGHTS_COMMAND;
        auto serverResponse = this->connectionManager->get(command);

        // Build a hash map for fast comparison of deleted devices
        std::unordered_map<std::string, Light> newRecord;
        // Get list of last devices seen on server
        auto pastRecordCopy = this->dataStore->getData();

        // For each light recently reported by the server
        for (auto& light : serverResponse.body)
        {
            std::string deviceId = light["id"];

            // Get more details on specific device (note: need to handle delete race condition)
            std::string command = GENERIC_LIGHTS_COMMAND + "/" + deviceId;
            auto detailedLight = this->connectionManager->get(command);
            // Check if light record was still found in the server by the time we wanted to get more
            // info on it
            if (detailedLight.returnCode == DEVICE_MISSING)
            {
                // Device not found on server anymore, likely deleted between query time
                continue;
            }

            // This could be optimized by finding the delta elements first and updating the object
            // rather then making a new one each time

            auto device = Light(detailedLight.body["brightness"], detailedLight.body["id"],
                                detailedLight.body["name"], detailedLight.body["on"],
                                detailedLight.body["room"]);

            // Check if device is new
            if (this->dataStore->newEntry(deviceId))
            {
                detailedLight.body["brightness"] = device.brightness;

                std::cout << detailedLight.body.dump(4) << std::endl;
            }

            else
            {
                // Existing device

                // Device is in past Record remove it from the copy map to see whats left for
                // deletion
                pastRecordCopy.erase(deviceId);

                // Device is found need to check if all info matches
                if (this->dataStore->getRecord(deviceId) != device)
                {
                    // Devices dont match find delta
                    auto pastDeviceRecord = this->dataStore->getRecord(deviceId);
                    // Find changes  TODO: make this a function
                    // In order of assumed highest likely hood
                    if (device.on != pastDeviceRecord.on)
                    {
                        json printData;
                        printData["id"] = deviceId;
                        printData["on"] = device.on;
                        std::cout << printData.dump(4) << std::endl;
                    }
                    if (device.brightness != pastDeviceRecord.brightness)
                    {
                        json printData;
                        printData["id"] = deviceId;
                        printData["brightness"] = device.brightness;
                        std::cout << printData.dump(4) << std::endl;
                    }
                    if (device.name != pastDeviceRecord.name)
                    {
                        json printData;
                        printData["id"] = deviceId;
                        printData["name"] = device.name;
                        std::cout << printData.dump(4) << std::endl;
                    }

                    if (device.room != pastDeviceRecord.room)
                    {
                        json printData;
                        printData["id"] = deviceId;
                        printData["room"] = device.room;
                        std::cout << printData.dump(4) << std::endl;
                    }
                }
            }

            // Add device to new master record
            newRecord.insert(std::make_pair(deviceId, device));
        }

        // If after all new devices have been removed from the past data struct any remaining were
        // in the past record but not the updated one, so must have been deleted
        for (auto device : pastRecordCopy)
        {
            // Deleted device
            std::cout << device.second.name << " (" << device.second.id << ") has been removed"
                      << std::endl;
        }

        // Update new device list
        this->dataStore->updateData(newRecord);

        // Delay time
        const auto endTime = std::chrono::high_resolution_clock::now();

        auto elapsedTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

        if (elapsedTime < targetDelta)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(targetDelta - elapsedTime));
        }
    }
}