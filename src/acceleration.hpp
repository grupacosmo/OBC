#ifndef OBC_ACCELERATION_HPP
#define OBC_ACCELERATION_HPP

#include <SparkFun_MMA8452Q.h>

#include <ctl/result.hpp>

#include "error.hpp"

namespace obc {

struct Acceleration {
    short x;
    short y;
    short z;
};

ctl::Result<ctl::Unit, Errc> init(MMA8452Q& accelerometer);
ctl::Result<Acceleration, Errc> measure(MMA8452Q& accelerometer);
void print(Acceleration acclr);

}  // namespace obc

#endif