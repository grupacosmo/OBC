#ifndef OBC_BAROMETER_HPP
#define OBC_BAROMETER_HPP

#include <BMP280.h>

#include "error.hpp"
#include "result.hpp"

namespace obc {

struct BmpMeasurements {
    double temperature;
    double pressure;
    double raw_altitude;
    double altitude_relative_to_ground;
};

double ground_pressure(BMP280& bmp);
Result<Unit, Errc> init(BMP280& bmp);
Result<BmpMeasurements, Errc> measure(BMP280& bmp);
void print(BmpMeasurements measurements);

}  // namespace obc

#endif