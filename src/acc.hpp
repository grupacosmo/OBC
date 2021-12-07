#include <Arduino.h>

#include "../lib/SparkFun_MMA8452Q/src/SparkFun_MMA8452Q.h"

#ifndef ACCEL_HPP
#define ACCEL_HPP

struct acceleration {
    short x;
    short y;
    short z;
};

struct accel_measurements_result {
    acceleration acclr;
    int err;
};

void accel_init();
accel_measurements_result accel_measurements();
inline void print_position(acceleration acclr)
{
    Serial.print(acclr.x);
    Serial.print("\t");
    Serial.print(acclr.y);
    Serial.print("\t");
    Serial.print(acclr.z);
    Serial.println();
}

#endif