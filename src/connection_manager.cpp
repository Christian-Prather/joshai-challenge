#include "interface/connection_manager.h"

ConnectionManager::ConnectionManager(std::string serverAddress, bool keepAlive)
{
    // Assign the server address as member variable incase needed to reference later
    this->serverAddress = serverAddress;
    // Make a connection using library specific mechanism
    this->connection = std::make_shared<httplib::Client>(serverAddress);

    // Tell the http connection to use the keep alive header
    this->connection->set_keep_alive(keepAlive);

    // Set timeout and connection failure times
    this->connection->set_connection_timeout(0, 300000); // 300 milliseconds
    this->connection->set_read_timeout(5, 0);            // 5 seconds
    this->connection->set_write_timeout(5, 0);           // 5 seconds
}

ServerResponse ConnectionManager::get(std::string& command)
{
    // Empty packet from server
    auto packet = ServerResponse();

    // TODO: handle errors
    auto response = this->connection->Get(command);

    // TODO: handle parse error for malformed data
    auto parsedData = json::parse(response->body);

    // Populate the packet with data from the server to be sent over to other subsystems
    packet.returnCode = response->status;
    packet.body = parsedData;
    return packet;
}