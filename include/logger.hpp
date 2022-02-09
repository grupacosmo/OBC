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

struct Packet {
    Acceleration acclr;
    BmpMeasurements measurements;
    // place for GPS String log
};

Result<Unit, Errc> init(File &);
String serialize(Packet &);
void file_appendln(const char *, const String &);
void file_appendln(const char *, const char *);
}  // namespace obc

#endif