#ifndef OBC_BAROMETER_HPP
#define OBC_BAROMETER_HPP
#define a_pr 1013.25 //ground level atmospheric pressure

#include <BMP280.h>

#include "error.hpp"

namespace obc
{
    using BMP = BMP280;

    struct Measurements
    {
        double temperature;
        double pressure;
        double altitude;
    };

    struct bmpMeasurements
    {
        Measurements measurements;
        Error bmp_err;
    };

    void init(BMP &bmp);
    bmpMeasurements bmp_measurements(BMP &bmp);
    void print_bmp_measurements(Measurements measurements);
}

#endif