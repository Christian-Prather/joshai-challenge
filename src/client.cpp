#include "interface/client.h"

HttpClient::HttpClient(std::string serverAddress, bool keepAlive)
{
    this->serverAddress = serverAddress;
    this->connection = std::make_shared<httplib::Client>(serverAddress);

    this->connection->set_keep_alive(keepAlive);

    this->connection->set_connection_timeout(0, 300000); // 300 milliseconds
    this->connection->set_read_timeout(5, 0);            // 5 seconds
    this->connection->set_write_timeout(5, 0);           // 5 seconds
}
