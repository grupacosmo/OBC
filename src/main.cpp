#include <Arduino.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "devices.hpp"
#include "gps.hpp"
#include "logger.hpp"

constexpr auto baud_rate = 9600l;
constexpr int interval = 2000;

String flight_path_folder;

HardwareSerial Serial3{PC11, PC10};
MMA8452Q accelerometer;
BMP280 bmp;
Adafruit_GPS gps{&Serial3};

uint32_t timer = millis();

File file;

void setup()
{
    Serial.begin(baud_rate);
    Serial.println("setup");

    obc::init();
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
