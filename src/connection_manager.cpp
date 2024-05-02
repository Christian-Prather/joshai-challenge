#include "interface/connection_manager.h"

ConnectionManager::ConnectionManager(std::string serverAddress, bool keepAlive)
{
    // Assign the server address as member variable incase needed to reference later
    this->serverAddress = serverAddress;
    // Make a connection using library specific mechanism
    try
    {
        this->connection = std::make_shared<httplib::Client>(serverAddress);
        // Tell the http connection to use the keep alive header
        this->connection->set_keep_alive(keepAlive);

        // Set timeout and connection failure times
        this->connection->set_connection_timeout(0, 300000); // 300 milliseconds
        this->connection->set_read_timeout(5, 0);            // 5 seconds
        this->connection->set_write_timeout(5, 0);           // 5 seconds
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

ServerResponse ConnectionManager::get(std::string& command)
{
    // Empty packet from server
    auto packet = ServerResponse();

    // catch client <-> Server connection errors
    try
    {
        auto response = this->connection->Get(command);
        if (response.error() != httplib::Error::Success)
        {
            std::cout << "Error: " << httplib::to_string(response.error()) << std::endl;
            exit(-1);
        }

        // handle parse error for malformed data
        try
        {
            // Library should throw parse errot
            auto parsedData = json::parse(response->body);

            // Populate the packet with data from the server to be sent over to other subsystems
            packet.returnCode = response->status;
            packet.body = parsedData;
            return packet;
        }
        catch (const json::parse_error& e)
        {
            std::cerr << "Could not parse json, data malformed " << e.what() << '\n';
            exit(-1);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(-1);
    }
}