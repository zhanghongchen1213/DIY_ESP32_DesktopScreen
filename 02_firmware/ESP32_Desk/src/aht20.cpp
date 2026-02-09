#include "aht20.h"

Adafruit_AHTX0 aht;

void AHT::init()
{
    Wire1.begin(21, 22);
    if (aht.begin(&Wire1))
    {
        Serial.println("AHT20 found!");
    }
    else
    {
        Serial.println("AHT20 not found!");
    }
};

std::map<String, int> AHT::getTemp()
{
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    std::map<String, int> result;
    result["temperature"] = temp.temperature;
    result["humidity"] = humidity.relative_humidity;

    return result;
};