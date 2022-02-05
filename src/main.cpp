#include <Arduino.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "gps.hpp"

constexpr auto baud_rate = 9600l;
constexpr int interval = 2000;
HardwareSerial Serial3{PC11, PC10};

MMA8452Q accelerometer;
BMP280 bmp;
Adafruit_GPS gps{&Serial3};

uint32_t timer = millis();

void setup()
{
    Serial.begin(baud_rate);
    Serial.println("setup");
    obc::init(accelerometer).expect("accelerometer init failure");
    obc::init(bmp).expect("BMP init failure");
    obc::init(gps).expect("GPS init failure");
}

void loop()
{
    if (timer - millis() > interval) {
        const auto acclr = obc::measure(accelerometer);
        const auto bmp_measurements = obc::measure(bmp);
        const auto gps_measurments = obc::measure(gps);
        if (acclr.is_ok()) { obc::print(acclr.unwrap()); }
        if (bmp_measurements.is_ok()) { obc::print(bmp_measurements.unwrap()); }
        if (gps_measurments.is_ok()) { obc::print(gps_measurments.unwrap()); }
        timer = millis();
    }
}
