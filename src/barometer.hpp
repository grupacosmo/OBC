#ifndef OBC_BAROMETER_HPP
#define OBC_BAROMETER_HPP

#include <BMP280.h>

#include <ccl/result.hpp>

#include "error.hpp"

namespace obc {

struct BmpMeasurements {
    double temperature;
    double pressure;
    double altitude;
};

ccl::Result<ccl::Unit, Errc> init(BMP280& bmp);
ccl::Result<BmpMeasurements, Errc> measure(BMP280& bmp);
void print(BmpMeasurements measurements);

}  // namespace obc

#endif