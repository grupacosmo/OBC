#ifndef OBC_ACCELERATION_HPP
#define OBC_ACCELERATION_HPP

#include <SparkFun_MMA8452Q.h>

#include "error.hpp"
#include "result.hpp"

namespace obc {

struct Acceleration {
    short x;
    short y;
    short z;
};

Result<Unit, Errc> init(MMA8452Q& accelerometer);
Result<Acceleration, Errc> measure(MMA8452Q& accelerometer);
void print(Acceleration acclr);

}  // namespace obc

#endif