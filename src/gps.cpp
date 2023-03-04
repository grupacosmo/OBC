#include "gps.hpp"

#include <Arduino.h>

namespace obc {

namespace {

constexpr auto baud_rate = 9600l;

}  // namespace

Result<Unit, Errc> init(Adafruit_GPS& gps)
{
    if (not gps.begin(baud_rate)) { return Err{ Errc::Busy }; }
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    gps.sendCommand(PGCMD_ANTENNA);
    Serial3.println(PMTK_Q_RELEASE);
    return Ok{ Unit{} };
}

Result<Unit, Errc> measure(Adafruit_GPS& gps)
{
    gps.read();
    if (gps.newNMEAreceived()) {
        if (not gps.parse(gps.lastNMEA())) { return Err{ Errc::Busy }; }
    }
    return Ok{ Unit{} };
}

GpsDate read_date(Adafruit_GPS& gps)
{
    return GpsDate{ gps.year, gps.month, gps.day };
}

GpsTime read_time(Adafruit_GPS& gps)
{
    return GpsTime{ gps.hour, gps.minute, gps.seconds, gps.milliseconds };
}

GpsPosition read_position(Adafruit_GPS& gps)
{
    if (gps.fix) {
        return GpsPosition{
            gps.fix,      gps.fixquality,      gps.longitudeDegrees,
            gps.lon,      gps.latitudeDegrees, gps.lat,
            gps.altitude, gps.speed,           gps.satellites
        };
    }
    return GpsPosition{ false, 0, 0, 0, 0, 0, 0, 0, 0 };
}

void print(GpsTime time)
{
    Serial.print("Time: ");
    if (not has_tens_digit(time.hour)) { Serial.print('0'); }
    Serial.print(time.hour);
    Serial.print(':');
    if (not has_tens_digit(time.minute)) { Serial.print('0'); }
    Serial.print(time.minute);
    Serial.print(':');
    if (not has_tens_digit(time.seconds)) { Serial.print('0'); }
    Serial.print(time.seconds);
    Serial.print('.');
    if (not has_hundreds_digit(time.milliseconds)) {
        // NOLINTNEXTLINE(bugprone-branch-clone)
        if (not has_tens_digit(time.milliseconds)) { Serial.print("00"); }
        else {
            Serial.print('0');
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
    if (not has_tens_digit(date.year)) { Serial.print("0"); }
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
        Serial.println(position.speed / mph_to_kph_conversion);
        Serial.print("Altitude: ");
        Serial.println(position.altitude);
        Serial.print("Satellites: ");
        Serial.println(static_cast<int>(position.satelites));
    }
}

}  // namespace obc
