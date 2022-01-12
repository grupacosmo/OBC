#include "gps.hpp"
#include <Arduino.h>


namespace obc{

    constexpr auto gps_baud_rate = 9600L;
    constexpr auto gps_delay = 1000L;
    void init(Gps& gps)
    {
        gps.begin(gps_baud_rate);
        gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
        gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
        gps.sendCommand(PGCMD_ANTENNA);
        delay(gps_delay);
        Serial3.println(PMTK_Q_RELEASE);
    }
    GpsMeasurments measure_gps(Gps& gps)
    {
        char c = gps.read();
            if (GPSECHO)
              if (gps.read() != 0) Serial.print(c);
          if (gps.newNMEAreceived()) {
            if (!gps.parse(gps.lastNMEA()))
            return {{},{},{},Error::Busy};
          }
        return {{gps.hour,gps.minute,gps.seconds,gps.milliseconds},
                {gps.year,gps.month,gps.day},
                {gps.fix,gps.fixquality,gps.longitudeDegrees,gps.lon,gps.latitudeDegrees,gps.lat,gps.altitude,gps.speed,gps.satellites},
                Error::Ok};

    }
    void print(const GpsTime& gps_time)
    {
        Serial.print("\nTime: ");
        if (gps_time.hour < 10) { Serial.print('0'); }
        Serial.print(gps_time.hour, DEC); Serial.print(':');
        if (gps_time.minute < 10) { Serial.print('0'); }
        Serial.print(gps_time.minute, DEC); Serial.print(':');
        if (gps_time.seconds < 10) { Serial.print('0'); }
        Serial.print(gps_time.seconds, DEC); Serial.print('.');
        if (gps_time.milliseconds < 10) {
          Serial.print("00");
        } else if (gps_time.milliseconds > 9 && gps_time.milliseconds < 100) {
          Serial.print("0");
        }
        Serial.println(gps_time.milliseconds);
    }
    void print(const GpsDate& gps_date)
    {
        Serial.print("Date: ");
        Serial.print(gps_date.day, DEC); Serial.print('/');
        Serial.print(gps_date.month, DEC); Serial.print("/20");
        Serial.println(gps_date.year, DEC);
      
    }
    void print(const GpsPosition& gps_position)
    {
        Serial.print("Fix: "); Serial.print((int)gps_position.fix);
        Serial.print(" quality: "); Serial.println((int)gps_position.fixquality);
        if (gps_position.fix) {
          Serial.print("Location: ");
          Serial.print(gps_position.latitude, 4); Serial.print(gps_position.lat);
          Serial.print(", ");
          Serial.print(gps_position.longitude, 4); Serial.println(gps_position.lon);
          Serial.print("Speed (km/h): "); Serial.println(gps_position.speed/1.85166);
          Serial.print("Altitude: "); Serial.println(gps_position.altitude);
          Serial.print("Satellites: "); Serial.println((int)gps_position.satelites);
        }
    }

}