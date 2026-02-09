#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <map>
#include <Wire.h>

extern Adafruit_AHTX0 aht;

class AHT
{
private:
public:
    void init();
    std::map<String, int> getTemp();
};