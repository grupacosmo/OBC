#include "lora.hpp"

namespace obc {

HardwareSerial Serial5(PD2, PC12);

constexpr auto baud_rate = 9600l;

Result<Unit, Errc> init_lora()
{
    Serial5.begin(baud_rate);
    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
    constexpr std::array commands = {
        "AT+DR=EU868",
        "AT+CH=NUM,0-2",
        "AT+MODE=LWOTAA",
        "AT+JOIN",
        "AT+UART=TIMEOUT,2000",
    };

    for (const auto command : commands) {
        Serial5.println(command);
        Serial.println("done");
        delay(10000);
        String payload = Serial5.readString();
        if (payload.startsWith("ERR")) { return Err{Errc::Busy}; }
    }
    return Ok{Unit{}};
}

DynamicJsonDocument to_json(const Packet &data)
{
    DynamicJsonDocument json(256);
    // json["AccX"] = data.acclr_measurements.x;
    // json["AccY"] = data.acclr_measurements.y;
    // json["AccZ"] = data.acclr_measurements.z;
    json["Alti"] = String(data.bmp_measurements.altitude, 2);
    // json["Fix"] = data.position.fix;
    // json["Pres"] = String(data.bmp_measurements.pressure, 2);
    // json["Loca"] =
    //     String(data.position.latitude) + String(data.position.longitude);
    // json["Sate"] = data.position.satelites;
    // json["Speed"] = data.position.speed / mph_to_kph_conversion;
    json["Temp"] = String(data.bmp_measurements.temperature, 2);

    return json;
}

String make_string_from_count(size_t count, char c)
{
    String result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) { result.concat(c); }
    return result;
}

String encode(const String &packet)
{
    // NOLINTNEXTLINE(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    String input = packet;
    // NOLINTNEXTLINE(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    const auto len = Base64.encodedLength(packet.length());
    auto result = make_string_from_count(len, ' ');
    // NOLINTNEXTLINE(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    Base64.encode(&result[0], &input[0], packet.length());
    return result;
}

void send_packet(const String &packet)
{
    auto encoded_logs = encode(packet);
    String encoded = String("AT+MSG=") + "\"" + encoded_logs + "\"";
    Serial5.print(encoded);
}

}  // namespace obc
