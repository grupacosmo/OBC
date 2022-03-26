#include "logger.hpp"

#include <Arduino.h>

constexpr uint8_t sd_chip_select = D9;

String flight_path_folder;

File file;

namespace obc {

Result<Unit, Errc> sd_init()
{
    unsigned int flight_id = 1;

    if (!SD.begin(sd_chip_select)) { return Err{Errc::Busy}; }

    file_appendln("/boot.txt", "SD card initialized properly.");

    while (SD.exists("/FLIGHT" + String(flight_id))) { flight_id++; }
    flight_path_folder = "/FLIGHT" + String(flight_id);

    SD.mkdir(flight_path_folder);

    file_appendln("/boot.txt", "Booting time: " + String(millis()) + "ms");

    String logs_legend =
        "Time\tSystem time\tFix\tQuality\tLocation\tSpeed (km/h)\t"
        "Altitude(gps)\tSatellites\tTemperature\tPressure\tAltitude(gps)\t"
        "Acceleration X\tAcceleration Y\tAcceleration Z";

    file_appendln("/logs.csv", logs_legend.c_str());

    return Ok{Unit{}};
}

void serialize_into(String& buf, const GpsTime& data)
{
    if (!has_tens_digit(data.hour)) { buf += '0'; }
    buf += data.hour;
    buf += ':';
    if (!has_tens_digit(data.minute)) { buf += '0'; }
    buf += data.minute;
    buf += ':';
    if (!has_tens_digit(data.seconds)) { buf += '0'; }
    buf += data.seconds;
    buf += ':';
    if (!has_hundreds_digit(data.milliseconds)) {
        if (!has_tens_digit(data.milliseconds)) { buf += "00"; }
        else {
            buf += '0';
        }
    }
    buf += data.milliseconds;
    buf += "\t";
    buf += millis();
    buf += "\t";
}

void serialize_into(String& buf, const GpsPosition& data)
{
    buf += static_cast<int>(data.fix);
    buf += "\t";
    buf += static_cast<int>(data.fixquality);
    buf += "\t";
    if (data.fix) {
        buf += String(data.latitude, 4);
        buf += data.lat;
        buf += ", ";
        buf += String(data.longitude, 4);
        buf += data.lon;
        buf += "\t";
        buf += data.speed / mph_to_kph_conversion;
        buf += "\t";
        buf += data.altitude;
        buf += "\t";
        buf += static_cast<int>(data.satelites);
        buf += "\t";
    }
    else {
        for (int i = 0; i < 5; ++i) { buf += "0\t"; }
    }
}

void serialize_into(String& buf, const GpsDate& data)
{
    buf += " Date: ";
    buf += data.day;
    buf += '/';
    buf += data.month;
    buf += "/20";
    if (!has_tens_digit(data.year)) { buf += '0'; }
    buf += data.year;
}

void serialize_into(String& buf, const BmpMeasurements& data)
{
    buf += data.temperature;
    buf += "\t";
    buf += data.pressure;
    buf += "\t";
    buf += data.altitude;
    buf += "\t";
}

void serialize_into(String& buf, const Acceleration& data)
{
    buf += data.x;
    buf += "\t";
    buf += data.y;
    buf += "\t";
    buf += data.z;
    buf += "\t";
}

void serialize_into(String& buf, const Packet& data)
{
    serialize_into(buf, data.time);
    serialize_into(buf, data.position);
    serialize_into(buf, data.bmp_measurements);
    serialize_into(buf, data.acclr_measurements);
}

void file_appendln(const char* file_name, const String& data)
{
    file_appendln(file_name, data.c_str());

}

void file_appendln(const char* file_name, const char* data)
{
    file =
        // NOLINTNEXTLINE (hicpp-signed-bitwise)
        SD.open(flight_path_folder + file_name, O_CREAT | O_WRITE | O_APPEND);
    file.println(data);
    file.close();
}

}  // namespace obc