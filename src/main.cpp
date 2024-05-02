// System libraries
#include <iostream>

// Local
#include "interface/connection_manager.h"
#include "interface/data_store.h"
#include "interface/light_interface.h"

// Third party
#include "tclap/CmdLine.h"

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

struct Arguments
{
    std::string serverIp;
    std::string portNumber;
    int pollHz;
    bool keepAlive;
};

Arguments parseArgs(int argc, char* argv[])
{
    TCLAP::CmdLine cmd("Josh.ai CLI", ' ', "1.0.0");

    // Command to pass in server ip
    TCLAP::ValueArg<std::string> hostArg("a", "server-address", "The server address.", false,
                                         "localhost", "string");

    // Command to pass in server port
    TCLAP::ValueArg<std::string> portArg("p", "port", "Port number.", false, "8080", "string");

    // Command to pass in poll rate. Default is -1 which is means dont wait at all go as fast as
    // possible.
    TCLAP::ValueArg<int> rateArg(
        "r", "rate",
        "Rate for client to poll server for updates. (Not a guarantee, if client can not keep up)",
        false, -1, "int");

    // Command to set keepAlive header or not
    TCLAP::SwitchArg keepAlive("k", "keep-alive",
                               "Make connections with keepAlive header, defaults to true", true);

    cmd.add(hostArg);
    cmd.add(portArg);
    cmd.add(rateArg);
    cmd.add(keepAlive);

    // Parse the argv array.
    cmd.parse(argc, argv);

    auto arguments = Arguments();
    arguments.serverIp = hostArg.getValue();
    arguments.portNumber = portArg.getValue();
    arguments.pollHz = rateArg.getValue();
    arguments.keepAlive = keepAlive;

    return arguments;
}

int main(int argc, char* argv[])
{
    // Assuming multiple interfaces on a single server abstract the client so a singleton will be
    // passed to all needed interfaces

    const auto args = parseArgs(argc, argv);

    // Set up the shared objects for the system
    auto clientConnection =
        std::make_shared<ConnectionManager>(args.serverIp + ":" + args.portNumber, args.keepAlive);
    auto dataStore = std::make_shared<DataStore<Light>>();

    LightsInterface lightsInterface(clientConnection, dataStore);

    // Run continuous monitoring (NOTE: If this were to expanded to multiple devices then the
    // interface run() should be threaded, right now it is blocking)
    lightsInterface.run(args.pollHz);

    return 0;
}