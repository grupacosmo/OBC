#include <Arduino.h>

#include "acceleration.hpp"
#include "barometer.hpp"

constexpr auto baud_rate = 9600L;
obc::Accelerometer acclrm;
obc::Bmp bmp;

void setup()
{
    Serial.begin(baud_rate);
    Serial.println("setup");
    obc::init(acclrm);
    obc::init(bmp);
}

void loop()
{
    const auto [acclr, acclr_err] = obc::measure(acclrm);
    const auto [measurements, bmp_err] = obc::measure(bmp);
    if (acclr_err == obc::Error::Ok) { obc::print(acclr); }
    if (bmp_err == obc::Error::Ok) { obc::print(measurements); }
}
