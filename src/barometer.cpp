#include "barometer.hpp"

#include <Arduino.h>

namespace obc {
void init(Bmp &bmp)
{
    Serial.println("Initializing BMP module");

    if (bmp.begin() == 0) {
        OBC_PANIC("BMP init failed");
    }
    else {
        Serial.println("BMP init success!\n\n");
    }

    bmp.setOversampling(4);
}

Result<BmpMeasurements, Errc> measure(Bmp &bmp)
{
    char result = bmp.startMeasurment();
    BmpMeasurements temp = {0, 0, 0};

    if (result == 0) { return Err{Errc::Busy}; }

    result = bmp.getTemperatureAndPressure(temp.temperature, temp.pressure);

    if (result == 0) { return Err{Errc::Busy}; }

    temp.altitude = bmp.altitude(temp.pressure, ground_lvl_pressure);

    return Ok{temp};
}

void print(BmpMeasurements measurements)
{
    Serial.print(measurements.temperature);
    Serial.print("\t");
    Serial.print(measurements.pressure);
    Serial.print("\t");
    Serial.print(measurements.altitude);
    Serial.println();
}
}  // namespace obc