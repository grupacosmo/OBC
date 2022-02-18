#ifndef OBC_ACCELERATION_HPP
#define OBC_ACCELERATION_HPP

#include <SparkFun_MMA8452Q.h>

#include <ccl/result.hpp>

#include "error.hpp"

namespace obc {

struct Acceleration {
    short x;
    short y;
    short z;
};

ccl::Result<ccl::Unit, Errc> init(MMA8452Q& accelerometer);
ccl::Result<Acceleration, Errc> measure(MMA8452Q& accelerometer);
void print(Acceleration acclr);

}  // namespace obc

#endif