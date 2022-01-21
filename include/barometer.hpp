#ifndef OBC_BAROMETER_HPP
#define OBC_BAROMETER_HPP

#include <BMP280.h>

#include "result.hpp"
#include "error.hpp"

namespace obc {
constexpr double ground_lvl_pressure = 1013.25;

using Bmp = BMP280;

struct BmpMeasurements {
    double temperature;
    double pressure;
    double altitude;
};

Result<Unit, Errc> init(Bmp& bmp);
Result<BmpMeasurements, Errc> measure(Bmp& bmp);
void print(BmpMeasurements measurements);
}  // namespace obc

#endif