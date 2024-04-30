// System libraries
#include <iostream>

// Local
#include "interface/client.h"
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

int main()
{
    std::cout << "Interface starting..." << std::endl;

    HttpClient client("localhost:8080", true);
    // auto response = cli.Get("/lights");

    // std::cout << response->status << std::endl;
    // std::cout << response->body << std::endl;

    return 0;
}