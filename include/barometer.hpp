#ifndef OBC_BAROMETER_HPP
#define OBC_BAROMETER_HPP

#include <BMP280.h>

#include "error.hpp"

namespace obc {
constexpr double ground_lvl_pressure = 1013.25;

using Bmp = BMP280;

struct BmpMeasurements {
    double temperature;
    double pressure;
    double altitude;
};

struct BmpMeasureResult {
    BmpMeasurements measurements;
    Error err;
};

void init(Bmp &bmp);
BmpMeasureResult measure(Bmp &bmp);
void print(BmpMeasurements measurements);
}  // namespace obc

#endif