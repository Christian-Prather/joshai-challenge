// File for the actuall http setup and interface
#pragma once

#include <memory>

#include "cpp-httplib/httplib.h"
#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

class HttpClient
{
  public:
    HttpClient(std::string serverAddress, bool keepAlive = true);

    // Returns sucess codes
    json get(std::string command);
    int push();

  private:
    std::string serverAddress;
    // Dont call http library directly, abstract the http library from the client interface to
    // change library if needed
    std::shared_ptr<httplib::Client> connection;
};