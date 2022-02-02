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
    obc::init(acclrm).expect("accelerometer init failure");
    obc::init(bmp).expect("BMP init failure");
    obc::init(gps).expect("GPS init failure");
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
