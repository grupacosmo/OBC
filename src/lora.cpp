#include "lora.hpp"

namespace obc {

constexpr auto baud_rate = 9600l;

HardwareSerial Serial5(PD2, PC12);

Result<Unit, Errc> init_lora()
{
    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
    constexpr std::array commands = {
        "AT+DR=EU868",
        "AT+CH=NUM,0-2",
        "AT+MODE=LWOTAA",
        "AT+JOIN"};
    Serial5.begin(baud_rate);

    for (const auto command : commands) {
        Serial5.println(command);
        delay(8000);
        String payload = Serial5.readString();
        if (payload.startsWith("ERR")) { return Err{Errc::Busy}; }
    }
    return Ok{Unit{}};
}

DynamicJsonDocument to_json(const Packet &data)
{
    DynamicJsonDocument json(256);
    json["AccX"] = data.acclr_measurements.x;
    json["AccY"] = data.acclr_measurements.y;
    json["AccZ"] = data.acclr_measurements.z;
    json["Alti"] = String(data.bmp_measurements.altitude, 2);
    json["Fix"] = data.position.fix;
    json["Pres"] = String(data.bmp_measurements.pressure, 2);
    json["Loca"] =
        String(data.position.latitude) + String(data.position.longitude);
    json["Sate"] = data.position.satelites;
    json["Speed"] = data.position.speed / mph_to_kph_conversion;
    json["Temp"] = String(data.bmp_measurements.temperature, 2);
    json["Time"] = String(data.date.year) + String(data.date.month)
                 + String(data.date.day) + String(data.time.hour)
                 + String(data.time.minute) + String(data.time.seconds)
                 + String(data.time.milliseconds);

    return json;
}

String json_to_str(const DynamicJsonDocument &json)
{
    return json.as<String>();
}

String encode(const DynamicJsonDocument &packet)
{
    auto payload = json_to_str(packet);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
    char encoded[512];
    Serial.println(&payload[0]);
    Serial.println(payload.length());

    Base64.encode(encoded, &payload[0], payload.length());  // NOLINT

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    return String(encoded);
}

void lora_serialize(const DynamicJsonDocument &packet)
{
    const auto encoded_logs = encode(packet);
    String encoded = String("AT+MSG=") + encoded_logs;
    Serial5.println(encoded);
}

}  // namespace obc
