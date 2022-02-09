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
void serialize_into(String &buf, const GpsMeasurements &data);
void serialize_into(String &buf, const BmpMeasurements &data);
void serialize_into(String &buf, const Acceleration &data);
void serialize_into(String &buf, const Packet &data);
template <typename T>
inline String serialize(const T &t)
{
    String buf;
    serialize_into(buf, t);
    return buf;
}

}  // namespace obc

#endif