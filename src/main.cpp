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

    if (const auto result = obc::init(acclrm); result.is_err()) {
        OBC_PANIC("accelerometer init failure");
    }

    if (const auto result = obc::init(bmp); result.is_err()) {
        OBC_PANIC("BMP init failure");
    }

    obc::init(gps);
}

void loop()
{
    if (timer - millis() > interval) {
        const auto acclr = obc::measure(acclrm);
        const auto bmp_measurements = obc::measure(bmp);
        const auto gps_measurments = obc::measure_gps(gps);
        if (acclr.is_ok()) { obc::print(acclr.unwrap()); }
        if (bmp_measurements.is_ok()) { obc::print(bmp_measurements.unwrap()); }
        if (gps_measurments.is_ok()) { obc::print(gps_measurments.unwrap()); }
        timer = millis();
    }
}
