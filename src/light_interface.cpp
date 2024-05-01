#include "interface/light_interface.h"

// TODO go through all code and make sure passing data efficiently

LightsInterface::LightsInterface(std::shared_ptr<ConnectionManager> connectionManager,
                                 std::shared_ptr<DataStore> dataStore)
{
    // Setup members
    this->connectionManager = connectionManager;
    this->dataStore = dataStore;
}

void buildDeviceObject()
{
    // Reused loop code in both getInitalStatus and run for building a device type

    // TODO in situations were the device was already seen by the progream and has not changed it
    // would be more efficent to just get the Light object already built in the data store rather
    // then create a new object, this would rewquire a check for devcice existin in map and if its
    // the same at this level however and that is currently handled in the data store
}

void LightsInterface::getInitalStatus()
{
    // Get data from server
    auto serverRespons = this->connectionManager->get("/lights");

    // Data structure for pretty printing. TODO: look at more efficient way to not reallocate memory
    // for the same data
    std::vector<json> formattedIntialData;

    // Go through each light reported by the server
    for (auto light : serverRespons.body)
    {
        // Get device id for reuse
        std::string deviceId = light["id"];

        // Get more details on specific device (note: need to handle delete race condition)
        std::string command = "/lights/" + deviceId;
        auto detailedLight = this->connectionManager->get(command);
        // Check if light record was still found in the server by the time we wanted to get more
        // info on it
        if (detailedLight.returnCode == 404)
        {
            // Device not found on server anymore, likely deleted between query time
            continue;
        }

        // If here the device is still seen by the server so construct and light specific object.
        // Note this auto creates the hash as well.
        auto device =
            Light(detailedLight.body["brightness"], detailedLight.body["id"],
                  detailedLight.body["name"], detailedLight.body["on"], detailedLight.body["room"]);

        // Add device to master record
        this->dataStore->addInitalDevice(device);
        // Update the brightness value to be percent for printing. (Note this happens automatically
        // when light object was created, just not for pretty printing)
        detailedLight.body["brightness"] = device.brightness;
        // Add light to printing structure
        formattedIntialData.push_back(detailedLight.body);
    }

    // TODO run through print class
    json jsonList(formattedIntialData);
    std::cout << jsonList.dump(4) << std::endl;
}

// TODO: Add hz argument to throttle connection
void LightsInterface::run()
{
    // Setup initial state records
    this->getInitalStatus();

    // Run until user terminates program and get updates on devices
    while (true)
    {
        auto serverResonse = this->connectionManager->get("/lights");

        // Build a hash map for fast comparison of deleted devices
        std::unordered_map<std::string, Light> newRecord;
        // Get list of last devices seen on server
        auto pastRecordCopy = this->dataStore->getData();

        // For each light recently reported by the server
        for (auto light : serverResonse.body)
        {
            std::string deviceId = light["id"];

            // Get more details on specific device (note: need to handle delete race condition)
            std::string command = "/lights/" + deviceId;
            auto detailedLight = this->connectionManager->get(command);
            // Check if light record was still found in the server by the time we wanted to get more
            // info on it
            if (detailedLight.returnCode == 404)
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
                // std::cout << "New device from server" << std::endl;
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
                if (this->dataStore->editedEntry(deviceId, device.hashString))
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
    }
}