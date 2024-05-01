// System libraries
#include <iostream>

// Local
#include "interface/connection_manager.h"
#include "interface/data_store.h"
#include "interface/light_interface.h"
/**
 * Steps
 * 1) Read state of all lights
 * 2) Condition data (brightness 0-255 to 0-100 etc)
 * 3) Print out to consol json data
 * 4) Monitor (pulling) for changes to the server
 * 5) Update consol with changes
 * 6) Handle errors (http fails, json fails) -> inform and recover
 * 7) Docs, Docs, Docs
 *
 * Thoughts:
 * Unit testing will be tricky as this would require a mock server in c++ or a guarantee that the
 * simulator is running.
 *
 * Need to make sure all edge cases are handled
 * (Maybe a basic event handler abstraction)
 *
 * ON/OFF events
 * Brightness event
 * ADD/Removal of light
 *
 */

// TODO set never exit flag
// TODO use cli library
// TODO get into json values
// TODO handle when server is down so no seggfault
// TODO: add tclap argument for ip and port, and keepAlive

// convertJsonDataFunciton()

int main()
{
    // Assuming multiple interfaces on a single server abstract the client so a singleton will be
    // passed to all needed interfaces

    // Set up the shared objects for the system
    auto clientConnection = std::make_shared<ConnectionManager>("localhost:8080", true);
    auto dataStore = std::make_shared<DataStore>();

    LightsInterface lightsInterface(clientConnection, dataStore);

    // Run continuous monitoring (NOTE: If this were to expanded to multiple devices then the
    // interface run() should be threaded, right now it is blocking)
    lightsInterface.run();

    return 0;
}