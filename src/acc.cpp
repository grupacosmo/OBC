#include "acc.hpp"

MMA8452Q accel;

void accel_init()
{
    Serial.println("MMA8452Q Orientation Test Code!");
    Wire.begin();

    if (!accel.begin()) {
        Serial.println(
            "Not Connected. Please check connections and read the hookup "
            "guide.");
        while (true) {}
    }
}

accel_measurements_result accel_measurements()
{
    if (accel.available() == 0) { return {{}, -1}; }
    return {{accel.getX(), accel.getY(), accel.getZ()}, 0};
}
