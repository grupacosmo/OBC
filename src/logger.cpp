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

String serialize(Packet &data)
{
    String logs;

    logs += data.measurements.temperature;
    logs += "\t";
    logs += data.measurements.pressure;
    logs += "\t";
    logs += data.measurements.altitude;
    logs += "\t";

    logs += data.acclr.x;
    logs += "\t";
    logs += data.acclr.y;
    logs += "\t";
    logs += data.acclr.z;
    logs += "\t";

    return logs;
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