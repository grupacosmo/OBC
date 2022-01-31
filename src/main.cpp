#include <Arduino.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "gps.hpp"

constexpr auto baud_rate = 9600L;
constexpr int interval = 2000;
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
    const auto [gpsmeasurments, err] = obc::measure_gps(gps);
    if (timer - millis() > interval) {
        const auto [acclr, acclr_err] = obc::measure(acclrm);
        const auto [measurements, bmp_err] = obc::measure(bmp);
        if (acclr_err == obc::Error::Ok) { obc::print(acclr); }
        if (bmp_err == obc::Error::Ok) { obc::print(measurements); }
        if (err == obc::Error::Ok) { obc::print(gpsmeasurments); }
        timer = millis();
    }
}
