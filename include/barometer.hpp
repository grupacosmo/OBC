#ifndef OBC_BAROMETER_HPP
#define OBC_BAROMETER_HPP

#include <BMP280.h>
#include <SD.h>

#include "error.hpp"
#include "result.hpp"

namespace obc {

struct BmpMeasurements {
    double temperature;
    double pressure;
    double altitude;
};

Result<Unit, Errc> init(BMP280& bmp);
Result<BmpMeasurements, Errc> measure(BMP280& bmp);
void print(BmpMeasurements measurements);

}  // namespace obc

#endif