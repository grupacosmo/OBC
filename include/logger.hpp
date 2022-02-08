#ifndef OBC_LOGGER_HPP
#define OBC_LOGGER_HPP

#include <Arduino.h>
#include <SD.h>
#include <Wire.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "error.hpp"
#include "gps.hpp"

namespace obc {
constexpr uint8_t cs = 4;

struct Packet {
    Acceleration acclr;
    BmpMeasurements measurements;
    // place for GPS String log
};

Result<Unit, Errc> init(File &);
String serialize(Packet &);
void write_file(const char *, const String &);
void write_file(const char *, const char *);
}  // namespace obc

#endif