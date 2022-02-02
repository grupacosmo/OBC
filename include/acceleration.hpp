#ifndef OBC_ACCELERATION_HPP
#define OBC_ACCELERATION_HPP

#include <SparkFun_MMA8452Q.h>

#include "result.hpp"
#include "error.hpp"

namespace obc {

using Accelerometer = MMA8452Q;

struct Acceleration {
    short x;
    short y;
    short z;
};

void init(Accelerometer& acclrm);
Result<Acceleration, Errc> measure(Accelerometer& acclrm);
void print(Acceleration acclr);

}  // namespace obc

#endif