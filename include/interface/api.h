#pragma once

#include "interface/client.h"
// File for the specific light interface. Decoupled so any new API can be hotswapped to work

// Handles the mapping of "getLightStatus()" calls to /lights api

class LightsInterface
{
  public:
    LightsInterface(std::shared_ptr<HttpClient> client);
    void getLightStatus();
    void getInitalStatus();

    void poll();

    // Make a map or just use the json type??
    json initalList;

  private:
    std::shared_ptr<HttpClient> client;
};