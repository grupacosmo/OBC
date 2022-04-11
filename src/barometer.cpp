#include "barometer.hpp"

#include <Arduino.h>

namespace obc {

namespace {

double ground_level_pressure;

}  // namespace

Result<Unit, Errc> init(BMP280& bmp)
{
    if (bmp.begin() == 0) { return Err{Errc::Busy}; }
    bmp.setOversampling(4);
    ground_level_pressure = ground_pressure(bmp);
    return Ok{Unit{}};
}

double ground_pressure(BMP280& bmp)
{
    double ground_pressure = 0;

    for (auto i = 0; i < 100; ++i) {
        const auto calibration = measure(bmp);
        ground_pressure += calibration.unwrap().raw_altitude;
        delay(50);
    }
    return ground_pressure /= 100;
}

Result<BmpMeasurements, Errc> measure(BMP280& bmp)
{
    char result = bmp.startMeasurment();
    BmpMeasurements temp = {0, 0, 0, 0};

    if (result == 0) { return Err{Errc::Busy}; }
    result = bmp.getTemperatureAndPressure(temp.temperature, temp.pressure);

    if (result == 0) { return Err{Errc::Busy}; }
    temp.raw_altitude = bmp.altitude(temp.pressure, ground_level_pressure);
    temp.altitude_relative_to_ground =
        temp.raw_altitude - ground_level_pressure;

    return Ok{temp};
}

void print(BmpMeasurements measurements)
{
    Serial.print(measurements.temperature);
    Serial.print("\t");
    Serial.print(measurements.pressure);
    Serial.print("\t");
    Serial.print(measurements.raw_altitude);
    Serial.print("\t");
    Serial.print(measurements.altitude_relative_to_ground);
    Serial.println();
}

}  // namespace obc