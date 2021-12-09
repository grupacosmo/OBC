#include <Arduino.h>

#include "acc.hpp"

constexpr auto baud_rate = 9600L;

void setup()
{
    Serial.begin(baud_rate);
    Serial.println("setup");
    obc::accel_init();
}

void loop()
{
    auto result = obc::accel_measurements();
    if (result.err == 0) { obc::print_position(result.acclr); }
}
