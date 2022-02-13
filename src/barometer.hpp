#ifndef OBC_BAROMETER_HPP
#define OBC_BAROMETER_HPP

#include <BMP280.h>

#include <ctl/result.hpp>

#include "error.hpp"

namespace obc {

struct BmpMeasurements {
    double temperature;
    double pressure;
    double altitude;
};

ctl::Result<ctl::Unit, Errc> init(BMP280& bmp);
ctl::Result<BmpMeasurements, Errc> measure(BMP280& bmp);
void print(BmpMeasurements measurements);

}  // namespace obc

#endif