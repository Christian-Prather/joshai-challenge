/**
 * @file light_interface.h
 * @author Christian Prather
 * @brief The device specific interface. Used to abstract away device specifics (in this case
 * lights) such as what request errors mean and how to structure / display specific data
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "interface/connection_manager.h"
#include "interface/data_store.h"

/**
 * @brief The LightInterface, this could be abstracted to be inherited from a base device class if
 * program was to be expanded
 *
 */
class LightsInterface
{
  public:
    /**
     * @brief Construct a new Lights Interface object
     *
     * @param connectionManager The client server connection
     * @param dataStore The internal subsystem data storage and sharing mechanism
     */
    LightsInterface(std::shared_ptr<ConnectionManager> connectionManager,
                    std::shared_ptr<DataStore<Light>> dataStore);

    /**
     * @brief The blocking function call to continuously update the system on changes to the server
     * regarding lights
     * @note If more then one device interface is used this will need to be made to run on a thread
     * and be non blocking to the main program
     */
    void run();

  private:
    /**
     * @brief Get the Inital Status object to track state changes
     *
     */
    void getInitalStatus();

    /// Reference to the server client connection
    std::shared_ptr<ConnectionManager> connectionManager;
    /// Reference to the dataStore
    std::shared_ptr<DataStore<Light>> dataStore;
};