#include "acceleration.hpp"

#include <Arduino.h>

namespace obc {

Result<Unit, Errc> init(MMA8452Q& accelerometer)
{
    Wire.begin();
    if (not accelerometer.begin()) { return Err{Errc::Busy}; }
    return Ok{Unit{}};
}

Result<Acceleration, Errc> measure(MMA8452Q& accelerometer)
{
    if (accelerometer.available() == 0) { return Err{Errc::Busy}; }
    return Ok{Acceleration{
        accelerometer.getX(),
        accelerometer.getY(),
        accelerometer.getZ()}};
}

void print(Acceleration acclr)
{
    Serial.print(acclr.x);
    Serial.print("\t");
    Serial.print(acclr.y);
    Serial.print("\t");
    Serial.print(acclr.z);
    Serial.println();
}

}  // namespace obc
