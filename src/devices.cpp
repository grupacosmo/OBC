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
        const String msg =
            String("Accelerometer not initialized properly, errc: ")
            + to_underlying(result.unwrap_err());
        file_appendln("/errors.txt", msg);
        panic(msg.c_str());
    }

    if (auto result = init(bmp); result.is_err()) {
        const String msg = String("Barometer not initialized properly, errc: ")
                         + to_underlying(result.unwrap_err());
        file_appendln("/errors.txt", msg);
        panic(msg.c_str());
    }

    if (auto result = init(gps); result.is_err()) {
        const String msg = String("GPS not initialized properly, errc: ")
                         + to_underlying(result.unwrap_err());
        file_appendln("/errors.txt", msg);
        panic(msg.c_str());
    }

    file_appendln("/boot.txt", "Devices initialized properly.");
}

}  // namespace obc