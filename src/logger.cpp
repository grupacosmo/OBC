#include "logger.hpp"

#include <Arduino.h>

constexpr uint8_t chip_select = 4;

extern String flight_path_folder;
extern File file;

unsigned int flight_id = 1;

namespace obc {

Result<Unit, Errc> init(File& boot)
{
    if (!SD.begin(chip_select)) { return Err{Errc::Busy}; }

    boot = SD.open((flight_path_folder + "/boot.txt"), O_WRITE);
    boot.println("SD not initialized.");
    boot.close();

    while (SD.exists("/FLIGHT_" + String(flight_id))) { flight_id++; }
    flight_path_folder = "/FLIGHT_" + String(flight_id);

    SD.mkdir(flight_path_folder);

    file_appendln("/boot.txt", "Booting time: " + String(millis()) + "ms");

    return Ok{Unit{}};
}

void serialize_into(String& buf, const GpsMeasurements& measurements)
{
    if (!has_tens_digit(measurements.time.hour)) { buf += '0'; }
    buf += measurements.time.hour;
    buf += ':';
    if (!has_tens_digit(measurements.time.minute)) { buf += '0'; }
    buf += measurements.time.minute;
    buf += ':';
    if (!has_tens_digit(measurements.time.seconds)) { buf += '0'; }
    buf += measurements.time.seconds;
    buf += ':';
    if (!has_hundreds_digit(measurements.time.milliseconds)) {
        if (!has_tens_digit(measurements.time.milliseconds)) { buf += "00"; }
        else {
            buf += '0';
        }
    }
    buf += measurements.time.milliseconds;
    buf += "\t";
    buf += "Fix: ";
    buf += static_cast<int>(measurements.position.fix);
    buf += " quality: ";
    buf += static_cast<int>(measurements.position.fixquality);
    buf += "\t";
    if (measurements.position.fix) {
        buf += "Location: ";
        buf += String(measurements.position.latitude, 4);
        buf += measurements.position.lat;
        buf += ", ";
        buf += String(measurements.position.longitude, 4);
        buf += measurements.position.lon;
        buf += "\t";
        buf += "Speed (km/h): ";
        buf += measurements.position.speed / velocity_conversion;
        buf += "\t";
        buf += "Altitude: ";
        buf += measurements.position.altitude;
        buf += "\t";
        buf += "Satellites: ";
        buf += static_cast<int>(measurements.position.satelites);
    }
}

void serialize_into(String& buf, const BmpMeasurements& measurements)
{
    buf += measurements.temperature;
    buf += "\t";
    buf += measurements.pressure;
    buf += "\t";
    buf += measurements.altitude;
    buf += "\t";
}

void serialize_into(String& buf, const Acceleration& measurements)
{
    buf += measurements.x;
    buf += "\t";
    buf += measurements.y;
    buf += "\t";
    buf += measurements.z;
    buf += "\t";
}

void serialize_into(String& buf, const Packet& data)
{
    serialize_into(buf, data.gps_measurements);
    serialize_into(buf, data.bmp_measurements);
    serialize_into(buf, data.acclr_measurements);
}

void date_append(const GpsDate& date)
{
    String date_;
    date_ += "Date: ";
    date_ += date.day;
    date_ += '/';
    date_ += date.month;
    date_ += "/20";
    if (!has_tens_digit(date.year)) { date_ += '0'; }
    date_ += date.year;

    file_appendln("/boot.txt", date_);
}

void file_appendln(const char* file_name, const String& data)
{
    file_appendln(file_name, data.c_str());
}

void file_appendln(const char* file_name, const char* data)
{
    file = SD.open(flight_path_folder + file_name, O_WRITE);
    file.println(data);
    file.close();
}

}  // namespace obc