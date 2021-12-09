#include "acceleration.hpp"

#include <Arduino.h>

namespace obc {

void init(Accelerometer& acclrm)
{
    Serial.println("MMA8452Q Orientation Test Code!");
    Wire.begin();

    if (!acclrm.begin()) {
        Serial.println(
            "Not Connected. Please check connections and read the hookup "
            "guide.");
        while (true) {}
    }
}

MeasureAccelerationResult measure_acceleration(Accelerometer& acclrm)
{
    if (acclrm.available() == 0) { return {{}, Error::Busy}; }
    return {{acclrm.getX(), acclrm.getY(), acclrm.getZ()}, Error::Ok};
}

void print_acceleration(Acceleration acclr)
{
    Serial.print(acclr.x);
    Serial.print("\t");
    Serial.print(acclr.y);
    Serial.print("\t");
    Serial.print(acclr.z);
    Serial.println();
}

}  // namespace obc
