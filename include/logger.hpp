#ifndef OBC_LOGGER_HPP
#define OBC_LOGGER_HPP

#include <Arduino.h>
#include <SD.h>
#include <Wire.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "error.hpp"
// place for GPS header file

namespace obc {
constexpr uint8_t cs = 4;
unsigned int flight_id = 1;

struct Packet {
    Acceleration acclr;
    BmpMeasurements measurements;
    // place for GPS String log
};

Result<Unit, Errc> init();
String serialize(Packet &);
void sd_dump(File &, String &);
}  // namespace obc

#endif