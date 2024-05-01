#include "interface/light.h"

Light::Light(uint8_t brightness, std::string id, std::string name, bool on, std::string room)
    : brightness(brightness), id(id), name(name), on(on), room(room)
{
    // Condition the data
    this->parseData();
    // Make device hash
    this->generateHash();
}

void Light::generateHash()
{
    // Use bool as string for hash
    std::string boolString;
    if (this->on)
    {
        boolString = "true";
    }
    else
    {
        boolString = "false";
    }

    // Hash based on all member values
    this->hashString = this->hasher(std::to_string(brightness) + id + name + room + boolString);
}

int Light::convertBrightness(int value)
{
    return (float(value / 255.0)) * 100;
}

void Light::parseData() { this->brightness = this->convertBrightness(this->brightness); }
