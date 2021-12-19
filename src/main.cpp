#include <Arduino.h>

#include "acceleration.hpp"

constexpr auto baud_rate = 9600L;
obc::Accelerometer acclrm;

void setup()
{
    Serial.begin(baud_rate);
    Serial.println("setup");
    obc::init(acclrm);
}

void loop()
{
    const auto [acclr, err] = obc::measure_acceleration(acclrm);
    if (err == obc::Error::Ok) { obc::print_acceleration(acclr); }
}
