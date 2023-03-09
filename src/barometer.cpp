#include "barometer.hpp"

#include <Arduino.h>

namespace obc {

namespace {

constexpr double ground_lvl_pressure = 1013.25;

}  // namespace

Result<Unit, Errc> init(BMP280& bmp)
{
    if (bmp.begin() == 0) { return Err{ Errc::Busy }; }
    bmp.setOversampling(4);
    return Ok{ Unit{} };
}

Result<BmpMeasurements, Errc> measure(BMP280& bmp)
{
    char result = bmp.startMeasurment();
    BmpMeasurements temp = { 0, 0, 0 };

    if (result == 0) { return Err{ Errc::Busy }; }
    result = bmp.getTemperatureAndPressure(temp.temperature, temp.pressure);

    if (result == 0) { return Err{ Errc::Busy }; }
    temp.altitude = bmp.altitude(temp.pressure, ground_lvl_pressure);

    return Ok{ temp };
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