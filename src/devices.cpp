#include "devices.hpp"

#include <Arduino.h>

extern MMA8452Q accelerometer;
extern BMP280 bmp;
extern Adafruit_GPS gps;

namespace obc {

void init()
{
    sd_init().expect("SD init failure");

    if (auto result = init(accelerometer); result.is_err()) {
        log_error_and_panic(
            String("Accelerometer not initialized properly, errc: ")
            + to_underlying(result.unwrap_err()));
    }

    if (auto result = init(bmp); result.is_err()) {
        log_error_and_panic(
            String("Barometer not initialized properly, errc: ")
            + to_underlying(result.unwrap_err()));
    }

    if (auto result = init(gps); result.is_err()) {
        log_error_and_panic(
            String("GPS not initialized properly, errc: ")
            + to_underlying(result.unwrap_err()));
    }

    if (auto result = init_lora(); result.is_err()) {
        log_error_and_panic(
            String("Lora not initialized properly, errc: ")
            + to_underlying(result.unwrap_err()));
    }

    log_boot("Devices initialized properly.");
}

}  // namespace obc