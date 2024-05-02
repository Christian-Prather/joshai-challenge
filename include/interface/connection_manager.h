#pragma once

/**
 * @file connection_manager.h
 * @author Christian Prather
 * @brief This is the abstraction / wrapper over the client server connection needed to get new
 * data. This is used so that the undelying connection protocol / library can be changed and not
 * break the rest of the system. Examples of different methods one might use are gRPC, websocket,
 * MQTT, or ROS. To swap this classes API would only need to be updated to the new call / library.
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <memory>

#include "cpp-httplib/httplib.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/**
 * @brief A unified data structure to capture data and status coming back from the server
 *
 */
struct ServerResponse
{
    /// Status code, matching typical http status codes (200 = OK), (404 = error)
    int returnCode;
    /// Response from server in json format, for http this is what comes over the wire for other
    /// protocols the data will need to be converted to pack it in json format
    json body;
};

/**
 * @brief The client server connection manager in charge of how the connection is established,
 * handling server response errors, and managing if a connection should be kept alive or reset
 *
 */
class ConnectionManager
{
  public:
    /**
     * @brief Construct a new Connection Manager object
     *
     * @param serverAddress IP address
     * @param keepAlive true if http connection should use keepAlive header
     */
    ConnectionManager(std::string serverAddress, bool keepAlive);

    /**
     * @brief Returns full response from server to a get command
     *
     * @param command String command ("/lights", "/lights/{id}")
     * @return ServerResponse Full response in json and status from server
     */
    ServerResponse get(std::string& command);

  private:
    /// IP address of server
    std::string serverAddress;

    // Dont call http library directly, abstract the http library from the client interface to
    // change library if needed
    /// Library/ protocol specific connection
    std::shared_ptr<httplib::Client> connection;
};