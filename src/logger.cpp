#include "logger.hpp"

#include <Arduino.h>

extern String flight_path_folder;
extern File file;

namespace obc {

Result<Unit, Errc> init(File &boot)
{
    if (!SD.begin()) { return Err{Errc::Busy}; }

    boot = SD.open((flight_path_folder + "/boot.txt"), O_WRITE);
    boot.println("SD not initialized.");
    boot.close();

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

void write_file(const char *file_name, const String &data)
{
    file = SD.open(flight_path_folder + file_name, O_WRITE);
    file.println(data);
    file.close();
}

void write_file(const char *file_name, const char *data)
{
    file = SD.open(flight_path_folder + file_name, O_WRITE);
    file.println(data);
    file.close();
}

}  // namespace obc