#ifndef OBC_GPS_HPP
#define OBC_GPS_HPP

#include <Adafruit_GPS.h>

#include <array>

#include "error.hpp"
#include "result.hpp"

namespace obc {

template <typename T>  // requires std::unsigned_integral<T>
bool has_tens_digit(T n)
{
    return n > 9;
}

template <typename T>  // requires std::unsigned_integral<T>
bool has_hundreds_digit(T n)
{
    return n > 99;
}

constexpr bool gpsecho = false;
constexpr double velocity_conversion = 1.85166;

using Gps = Adafruit_GPS;
using GpsFloat = nmea_float_t;

struct GpsTime {
    uint8_t hour;
    uint8_t minute;
    uint8_t seconds;
    uint16_t milliseconds;
};

struct GpsDate {
    uint8_t year;
    uint8_t month;
    uint8_t day;
};

struct GpsPosition {
    bool fix;
    uint8_t fixquality;
    GpsFloat longitude;
    char lon;
    GpsFloat latitude;
    char lat;
    GpsFloat altitude;
    GpsFloat speed;
    uint8_t satelites;
};

struct GpsMeasurments {
    GpsTime time;
    GpsDate date;
    GpsPosition gpsposition;
};

Result<Unit, Errc> init(Gps& gps);
Result<GpsMeasurments, Errc> measure_gps(Gps& gps);
void print(GpsMeasurments gps_measurments);
}  // namespace obc

#endif