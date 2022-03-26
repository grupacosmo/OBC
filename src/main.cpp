#include <Arduino.h>
#include <ArduinoJson.h>
#include "acceleration.hpp"
#include "barometer.hpp"
#include "devices.hpp"
#include "gps.hpp"
#include "logger.hpp"
#include "lora.hpp"
constexpr auto baud_rate = 9600l;
constexpr int interval = 2000;

bool is_date_appended = false;

HardwareSerial Serial3(PC11, PC10);
MMA8452Q accelerometer;
BMP280 bmp;
Adafruit_GPS gps(&Serial3);

uint32_t timer = millis();

void setup()
{
    Serial.begin(baud_rate);
    Serial.println("setup");
    auto a = obc::init_lora();
    obc::init();
}

void loop()
{
    if (obc::measure(gps).is_ok() && millis() - timer > interval) {
        obc::Packet logs = {{}, {}, {}, {}, {}};
        const auto acclr = obc::measure(accelerometer);
        const auto bmp_measurements = obc::measure(bmp);
        logs.time = obc::read_time(gps);
        logs.position = obc::read_position(gps);

        if (acclr.is_ok()) { logs.acclr_measurements = acclr.unwrap(); }
        if (bmp_measurements.is_ok()) {
            logs.bmp_measurements = bmp_measurements.unwrap();
        }

        if (!is_date_appended) {
            obc::file_appendln(
                "/boot.txt",
                obc::serialize(obc::read_date(gps)));
            is_date_appended = true;
        }
        Serial.println("Hej");
        obc::lora_serialize(logs);
        obc::file_appendln("/logs.csv", obc::serialize(logs));

        timer = millis();
    }
}
