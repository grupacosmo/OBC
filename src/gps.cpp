#include "gps.hpp"

#include <Arduino.h>

namespace obc {

namespace {

constexpr auto baud_rate = 9600l;

constexpr std::size_t airborne_hex_array_size = 52;

constexpr std::array<const char, airborne_hex_array_size> airborne_mode_set = {
    0xb5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xff, 0xff, 0x07, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xfa, 0x00,
    0xfa, 0x00, 0x64, 0x00, 0x2c, 0x01, 0x00, 0x3c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0xe9,
    0xb5, 0x62, 0x06, 0x24, 0x00, 0x00, 0x2a, 0x84
};

}  // namespace

Result<Unit, Errc> init(Adafruit_GPS& gps)
{
    if (not gps.begin(baud_rate)) { return Err{ Errc::Busy }; }
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    gps.sendCommand(PGCMD_ANTENNA);
    Serial3.println(PMTK_Q_RELEASE);
    Serial3.write(airborne_mode_set.data(), airborne_mode_set.size());
    Serial3.write("\0");

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
        // clang-format off
        return GpsPosition{
            gps.fix,
            gps.fixquality,
            gps.longitudeDegrees,
            gps.lon,
            gps.latitudeDegrees,
            gps.lat,
            gps.altitude,
            gps.speed,
            gps.satellites,
        };
        // clang-format on
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
