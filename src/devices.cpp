#include "devices.hpp"

#include <Arduino.h>

extern File file;
extern MMA8452Q accelerometer;
extern BMP280 bmp;
extern Adafruit_GPS gps;

namespace obc {
void init()
{
    init(file).expect("SD init failure");

    if (auto result = init(accelerometer); result.is_err()) {
        file_appendln(
            "/errors.txt",
            String("Accelerometer not initialized properly, errc: ")
                + to_underlying(result.unwrap_err()));
        panic("Accel Error.");
    }

    if (auto result = init(bmp); result.is_err()) {
        file_appendln(
            "/errors.txt",
            String("Barometer not initialized properly, errc: ")
                + to_underlying(result.unwrap_err()));
    }

    if (auto result = init(gps); result.is_err()) {
        file_appendln(
            "/errors.txt",
            String("GPS not initialized properly, errc: ")
                + to_underlying(result.unwrap_err()));
    }

    obc::file_appendln("/boot.txt", "Devices initialized without problems.");
}
}  // namespace obc