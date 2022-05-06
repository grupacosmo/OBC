#ifndef OBC_LOGGER_HPP
#define OBC_LOGGER_HPP

#include <Arduino.h>
#include <SD.h>
#include <Wire.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "buzzer.hpp"
#include "digits.hpp"
#include "error.hpp"
#include "gps.hpp"
#include "source_location.hpp"

namespace obc {

struct Packet {
    Acceleration acclr_measurements;
    BmpMeasurements bmp_measurements;
    GpsTime time;
    GpsDate date;
    GpsPosition position;
};

Result<Unit, Errc> sd_init();

void log_boot(const char *msg);
inline void log_boot(const String &msg) { log_boot(msg.c_str()); }

void log_data(const char *msg);
inline void log_data(const String &msg) { log_data(msg.c_str()); }

void log_error(const char *msg);
inline void log_error(const String &msg) { log_error(msg.c_str()); }

void log_error_and_panic(
    const char *msg,
    SourceLocation loc = SourceLocation::current());

inline void log_error_and_panic(
    const String &msg,
    SourceLocation loc = SourceLocation::current())
{
    log_error_and_panic(msg.c_str(), loc);
}

void serialize_into(String &buf, const BmpMeasurements &data);
void serialize_into(String &buf, const Acceleration &data);
void serialize_into(String &buf, const Packet &data);
void serialize_into(String &buf, const GpsTime &data);
void serialize_into(String &buf, const GpsPosition &data);
void serialize_into(String &buf, const GpsDate &data);

template <typename T>
inline String serialize(const T &t)
{
    String buf;
    serialize_into(buf, t);
    return buf;
}

}  // namespace obc

#endif