#ifndef OBC_GPS_HPP
#define OBC_GPS_HPP

#include <Adafruit_GPS.h>

#include <array>

#include "digits.hpp"
#include "error.hpp"
#include "result.hpp"

constexpr nmea_float_t mph_to_kph_conversion = 1.85166f;

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

Result<Unit, Errc> init(Adafruit_GPS& gps);
Result<Unit, Errc> measure(Adafruit_GPS& gps);
GpsDate read_date(Adafruit_GPS& gps);
GpsTime read_time(Adafruit_GPS& gps);
GpsPosition read_position(Adafruit_GPS& gps);

}  // namespace obc

#endif