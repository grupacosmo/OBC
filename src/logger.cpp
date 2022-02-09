#include "logger.hpp"

#include <Arduino.h>

constexpr uint8_t chip_select = 4;

extern String flight_path_folder;
extern File file;

unsigned int flight_id = 1;

namespace obc {

Result<Unit, Errc> init(File &boot)
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

String serialize(const Packet &data)
{
    String logs;

    if (!has_tens_digit(data.gps_measurements.time.hour)) { logs += '0'; }
    logs += data.gps_measurements.time.hour;
    logs += ':';
    if (!has_tens_digit(data.gps_measurements.time.minute)) { logs += '0'; }
    logs += data.gps_measurements.time.minute;
    logs += ':';
    if (!has_tens_digit(data.gps_measurements.time.seconds)) { logs += '0'; }
    logs += data.gps_measurements.time.seconds;
    logs += ':';
    if (!has_hundreds_digit(data.gps_measurements.time.milliseconds)) {
        if (!has_tens_digit(data.gps_measurements.time.milliseconds)) {
            logs += "00";
        }
        else {
            logs += '0';
        }
    }
    logs += data.gps_measurements.time.milliseconds;
    logs += "\t";

    logs += "Fix: ";
    logs += static_cast<int>(data.gps_measurements.position.fix);
    logs += " quality: ";
    logs += static_cast<int>(data.gps_measurements.position.fixquality);
    logs += "\t";
    if (data.gps_measurements.position.fix) {
        logs += "Location: ";
        logs += String(data.gps_measurements.position.latitude, 4);
        logs += data.gps_measurements.position.lat;
        logs += ", ";
        logs += String(data.gps_measurements.position.longitude, 4);
        logs += data.gps_measurements.position.lon;
        logs += "\t";
        logs += "Speed (km/h): ";
        logs += data.gps_measurements.position.speed / velocity_conversion;
        logs += "\t";
        logs += "Altitude: ";
        logs += data.gps_measurements.position.altitude;
        logs += "\t";
        logs += "Satellites: ";
        logs += static_cast<int>(data.gps_measurements.position.satelites);
    }

    logs += data.bmp_measurements.temperature;
    logs += "\t";
    logs += data.bmp_measurements.pressure;
    logs += "\t";
    logs += data.bmp_measurements.altitude;
    logs += "\t";

    logs += data.acclr_measurements.x;
    logs += "\t";
    logs += data.acclr_measurements.y;
    logs += "\t";
    logs += data.acclr_measurements.z;
    logs += "\t";

    return logs;
}

void date_append(const GpsDate &date)
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

void file_appendln(const char *file_name, const String &data)
{
    file_appendln(file_name, data.c_str());
}

void file_appendln(const char *file_name, const char *data)
{
    file = SD.open(flight_path_folder + file_name, O_WRITE);
    file.println(data);
    file.close();
}

}  // namespace obc