#ifndef OBC_LOGGER_HPP
#define OBC_LOGGER_HPP

#include <Arduino.h>
#include <SD.h>
#include <Wire.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "digits.hpp"
#include "error.hpp"
#include "gps.hpp"

namespace obc {

struct Packet {
    Acceleration acclr_measurements;
    BmpMeasurements bmp_measurements;
    GpsMeasurements gps_measurements;
};

Result<Unit, Errc> init(File &);
String serialize(const Packet &);
void file_appendln(const char *, const String &);
void file_appendln(const char *, const char *);
void date_append(const GpsDate &date);

}  // namespace obc

#endif