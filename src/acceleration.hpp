#ifndef OBC_ACCELERATION_HPP
#define OBC_ACCELERATION_HPP

#include <SparkFun_MMA8452Q.h>

#include "error.hpp"

namespace obc {

using Accelerometer = MMA8452Q;

struct Acceleration {
    short x;
    short y;
    short z;
};

struct MeasureAccelerationResult {
    Acceleration acclr;
    Error err;
};

void init(Accelerometer& acclrm);
MeasureAccelerationResult measure_acceleration(Accelerometer& acclrm);
void print_acceleration(Acceleration acclr);

}  // namespace obc

#endif