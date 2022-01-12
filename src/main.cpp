#include <Arduino.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "gps.hpp"

constexpr auto baud_rate = 9600L;
HardwareSerial Serial3(PC11, PC10);
obc::Accelerometer acclrm;
obc::Bmp bmp;
obc::Gps gps(&Serial3);

uint32_t timer = millis();


void setup()
{
    Serial.begin(baud_rate);
    Serial.println("setup");
    obc::init(acclrm);
    obc::init(bmp);
    obc::init(gps);
}

void loop()
{
    constexpr int gps_interval = 2000;
    const auto [acclr, acclr_err] = obc::measure(acclrm);
    const auto [measurements, bmp_err] = obc::measure(bmp);
    const auto [time,date,gpsposition,err] = obc::measure_gps(gps);
    if (acclr_err == obc::Error::Ok) { obc::print(acclr); }
    if (bmp_err == obc::Error::Ok) { obc::print(measurements); }
    if(err == obc::Error::Ok && timer - millis() > gps_interval)
    {
        obc::print(time);
        obc::print(date);
        obc::print(gpsposition);
        timer = millis();
    }
}
