#include "gps.hpp"

#include <Arduino.h>

namespace obc {

constexpr auto gps_baud_rate = 9600L;
void init(Gps& gps)
{
    gps.begin(gps_baud_rate);
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    gps.sendCommand(PGCMD_ANTENNA);
    Serial3.println(PMTK_Q_RELEASE);
}
Result<GpsMeasurments, Errc> measure_gps(Gps& gps)
{
    char c = gps.read();
    if (gpsecho && gps.read() != 0) Serial.print(c);
    if (gps.newNMEAreceived() && !gps.parse(gps.lastNMEA())) {
        return Err{Errc::Busy};
    }
    return Ok{GpsMeasurments{
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
void print(GpsMeasurments gps_measurments)
{
    // printing time
    Serial.print("\nTime: ");
    if (!has_tens_digit(gps_measurments.time.hour)) { Serial.print('0'); }
    Serial.print(gps_measurments.time.hour, DEC);
    Serial.print(':');
    if (!has_tens_digit(gps_measurments.time.minute)) { Serial.print('0'); }
    Serial.print(gps_measurments.time.minute, DEC);
    Serial.print(':');
    if (!has_tens_digit(gps_measurments.time.seconds)) { Serial.print('0'); }
    Serial.print(gps_measurments.time.seconds, DEC);
    Serial.print('.');
    if (!has_hundreds_digit(gps_measurments.time.milliseconds)) {
        if (!has_tens_digit(gps_measurments.time.milliseconds)) {
            Serial.print("00");
        }
        else
            Serial.print("0");
    }
    Serial.println(gps_measurments.time.milliseconds);

    // printing date
    Serial.print("Date: ");
    Serial.print(gps_measurments.date.day, DEC);
    Serial.print('/');
    Serial.print(gps_measurments.date.month, DEC);
    Serial.print("/20");
    Serial.println(gps_measurments.date.year, DEC);

    // printing position
    Serial.print("Fix: ");
    Serial.print((int)gps_measurments.gpsposition.fix);
    Serial.print(" quality: ");
    Serial.println((int)gps_measurments.gpsposition.fixquality);
    if (gps_measurments.gpsposition.fix) {
        Serial.print("Location: ");
        Serial.print(gps_measurments.gpsposition.latitude, 4);
        Serial.print(gps_measurments.gpsposition.lat);
        Serial.print(", ");
        Serial.print(gps_measurments.gpsposition.longitude, 4);
        Serial.println(gps_measurments.gpsposition.lon);
        Serial.print("Speed (km/h): ");
        Serial.println(gps_measurments.gpsposition.speed / velocity_conversion);
        Serial.print("Altitude: ");
        Serial.println(gps_measurments.gpsposition.altitude);
        Serial.print("Satellites: ");
        Serial.println((int)gps_measurments.gpsposition.satelites);
    }
}
}  // namespace obc