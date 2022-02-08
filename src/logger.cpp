#include "logger.hpp"

#include <Arduino.h>

namespace obc {
extern String flight_path_folder;

Result<Unit, Errc> init()
{
    if (!SD.begin()) { return Err{Errc::Busy}; }

    return Ok{Unit{}};
}

String log(BmpMeasurements measurements)
{
    String temp;
    temp += measurements.temperature;
    temp += "\t";
    temp += measurements.pressure;
    temp += "\t";
    temp += measurements.altitude;
    temp += "\t";

    return temp;
}

String log(Acceleration acclr)
{
    String temp;
    temp += acclr.x;
    temp += "\t";
    temp += acclr.y;
    temp += "\t";
    temp += acclr.z;
    temp += "\t";

    return temp;
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

void sd_dump(File &telemetry, String &logs)
{
    telemetry = SD.open("telemetry.txt", O_WRITE);
    telemetry.println(logs);
    telemetry.close();
}

}  // namespace obc