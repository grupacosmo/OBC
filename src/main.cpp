#include <Arduino.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "gps.hpp"
#include "logger.hpp"
#include "utils.hpp"

constexpr auto baud_rate = 9600l;
constexpr int interval = 2000;
unsigned int flight_id = 1;

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

    while (SD.exists("/FLIGHT_" + String(flight_id))) { flight_id++; }
    flight_path_folder = "/FLIGHT_" + String(flight_id);
    SD.mkdir(flight_path_folder);
    file = SD.open((flight_path_folder + "/boot.txt"), O_WRITE);
    file.println("Booting time: " + String(millis()) + "ms");
    file.close();

    obc::init(file).expect("SD init failure");

    if (auto result = obc::init(accelerometer); result.is_err()) {
        obc::write_file(
            "/errors.txt",
            "Accelerometer not initialized properly, errc: "
                + obc::to_underlying(result.unwrap_err()));
        obc::panic("Accel Error.");
    }

    if (auto result = obc::init(bmp); result.is_err()) {
        obc::write_file(
            "/errors.txt",
            "Barometer not initialized properly, errc: "
                + obc::to_underlying(result.unwrap_err()));
    }

    if (auto result = obc::init(gps); result.is_err()) {
        obc::write_file(
            "/errors.txt",
            "GPS not initialized properly, errc: "
                + obc::to_underlying(result.unwrap_err()));
    }

    obc::write_file("/boot.txt", "Devices initialized without problems.");
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
