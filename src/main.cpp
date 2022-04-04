#include <Arduino.h>
#include <IWatchdog.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "devices.hpp"
#include "gps.hpp"
#include "logger.hpp"
#include "lora.hpp"

constexpr auto baud_rate = 9600l;
constexpr auto logs_interval = 2000;
constexpr auto lora_interval = 60000;
constexpr auto watchdog_interval = 10000000;

bool is_date_appended = false;

HardwareSerial Serial3(PC11, PC10);
MMA8452Q accelerometer;
BMP280 bmp;
Adafruit_GPS gps(&Serial3);

uint32_t timer = millis();
uint32_t lora_timer = millis();

void setup()
{
    Serial.begin(baud_rate);
    obc::init();
    IWatchdog.begin(watchdog_interval);
}

void loop()
{
    if (obc::measure(gps).is_ok() && millis() - timer > logs_interval) {
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
            obc::log_boot(obc::serialize(obc::read_date(gps)));
            is_date_appended = true;
        }

        obc::log_data(obc::serialize(logs));

        if (millis() - lora_timer > lora_interval) {
            obc::send_packet(obc::lora_packet(logs));
            lora_timer = millis();
        }

        obc::buzzer();
        timer = millis();
        IWatchdog.reload();
    }
}
