#include <Arduino.h>

#include "acceleration.hpp"
#include "barometer.hpp"

constexpr auto baud_rate = 9600L;
obc::Accelerometer acclrm;
obc::BMP bmp;

void setup()
{
    Serial.begin(baud_rate);
    Serial.println("setup");
    obc::init(acclrm);
    obc::init(bmp);
}

void loop()
{
    const auto [acclr, acclrm_err] = obc::measure_acceleration(acclrm);
    const auto [measurements, bmp_err] = obc::bmp_measurements(bmp);
    if (acclrm_err == obc::Error::Ok && bmp_err == obc::Error::Ok) 
    { 
        obc::print_acceleration(acclr); 
        obc::print_bmp_measurements(measurements);
    }
}
