#ifndef OBC_GPS_HPP
#define OBC_GPS_HPP

#include <Adafruit_GPS.h>

#include <array>
#include <ccl/result.hpp>

#include "error.hpp"

namespace obc {

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
    nmea_float_t longitude;
    char lon;
    nmea_float_t latitude;
    char lat;
    nmea_float_t altitude;
    nmea_float_t speed;
    uint8_t satelites;
};

struct GpsMeasurements {
    GpsTime time;
    GpsDate date;
    GpsPosition position;
};

ccl::Result<ccl::Unit, Errc> init(Adafruit_GPS& gps);
ccl::Result<GpsMeasurements, Errc> measure(Adafruit_GPS& gps);
void print(GpsMeasurements measurements);

}  // namespace obc

#endif