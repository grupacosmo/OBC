#ifndef OBC_GPS_HPP
#define OBC_GPS_HPP

#include <Adafruit_GPS.h>

#include "error.hpp"

namespace obc {

constexpr bool GPSECHO = false;

using Gps = Adafruit_GPS;
using Gps_float = nmea_float_t;

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
    Gps_float longitude;
    char lon;
    Gps_float latitude;
    char lat;
    Gps_float altitude;
    Gps_float speed;
    uint8_t satelites;
};

struct GpsMeasurments {
    GpsTime time;
    GpsDate date;
    GpsPosition gpsposition;
    Error err;
};

void init(Gps& gps);
GpsMeasurments measure_gps(Gps& gps);
void print(const GpsTime& gps_time);
void print(const GpsDate& gps_date);
void print(const GpsPosition& gps_position);
}  // namespace obc

#endif