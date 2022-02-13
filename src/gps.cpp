#include "gps.hpp"

#include <Arduino.h>

using namespace ctl;

namespace obc {

namespace {

constexpr auto baud_rate = 9600l;
constexpr nmea_float_t velocity_conversion = 1.85166f;

}  // namespace

Result<Unit, Errc> init(Adafruit_GPS& gps)
{
    if (!gps.begin(baud_rate)) { return Err{Errc::Busy}; }
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    gps.sendCommand(PGCMD_ANTENNA);
    Serial3.println(PMTK_Q_RELEASE);
    return Ok{Unit{}};
}

Result<GpsMeasurements, Errc> measure(Adafruit_GPS& gps)
{
    gps.read();
    gps.read();
    if (gps.newNMEAreceived() && !gps.parse(gps.lastNMEA())) {
        return Err{Errc::Busy};
    }
    return Ok{GpsMeasurements{
        {gps.hour, gps.minute, gps.seconds, gps.milliseconds},
        {gps.year, gps.month, gps.day},
        {gps.fix,
         gps.fixquality,
         gps.longitudeDegrees,
         gps.lon,
         gps.latitudeDegrees,
         gps.lat,
         gps.altitude,
         gps.speed,
         gps.satellites}}};
}

namespace {

template <typename T>  // requires std::unsigned_integral<T>
constexpr bool has_tens_digit(T n)
{
    return n > 9;
}

template <typename T>  // requires std::unsigned_integral<T>
constexpr bool has_hundreds_digit(T n)
{
    return n > 99;
}

}  // namespace

void print(GpsTime time)
{
    Serial.print("Time: ");
    if (!has_tens_digit(time.hour)) { Serial.print('0'); }
    Serial.print(time.hour);
    Serial.print(':');
    if (!has_tens_digit(time.minute)) { Serial.print('0'); }
    Serial.print(time.minute);
    Serial.print(':');
    if (!has_tens_digit(time.seconds)) { Serial.print('0'); }
    Serial.print(time.seconds);
    Serial.print('.');
    if (!has_hundreds_digit(time.milliseconds)) {
        if (!has_tens_digit(time.milliseconds)) { Serial.print("00"); }
        else {
            Serial.print("0");
        }
    }
    Serial.println(time.milliseconds);
}

void print(GpsDate date)
{
    Serial.print("Date: ");
    Serial.print(date.day);
    Serial.print('/');
    Serial.print(date.month);
    Serial.print("/20");
    if (!has_tens_digit(date.year)) { Serial.print("0"); }
    Serial.println(date.year);
}

void print(GpsPosition position)
{
    Serial.print("Fix: ");
    Serial.print(static_cast<int>(position.fix));
    Serial.print(" quality: ");
    Serial.println(static_cast<int>(position.fixquality));
    if (position.fix) {
        Serial.print("Location: ");
        Serial.print(position.latitude, 4);
        Serial.print(position.lat);
        Serial.print(", ");
        Serial.print(position.longitude, 4);
        Serial.println(position.lon);
        Serial.print("Speed (km/h): ");
        Serial.println(position.speed / velocity_conversion);
        Serial.print("Altitude: ");
        Serial.println(position.altitude);
        Serial.print("Satellites: ");
        Serial.println(static_cast<int>(position.satelites));
    }
}

void print(GpsMeasurements measurements)
{
    obc::print(measurements.time);
    obc::print(measurements.date);
    obc::print(measurements.position);
}

}  // namespace obc
