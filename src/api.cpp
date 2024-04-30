#include "interface/api.h"

#include <iostream>

LightsInterface::LightsInterface(std::shared_ptr<HttpClient> client)
{
    this->client = client;
    std::cout << "Light Interface created" << std::endl;
}

void LightsInterface::getInitalStatus()
{

    auto response = this->client->get("/lights");
    this->initalList = response;
    for (auto light : response)
    {
        std::cout << light["id"] << std::endl;
    }
    std::cout << this->initalList << std::endl;
}

void LightsInterface::getLightStatus() { this->client->get("/lights"); }

void LightsInterface::poll()
{
    std::cout << "Polling" << std::endl;
    while (true)
    {
    }
}