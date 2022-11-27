#include "lora.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace obc {

namespace {

HardwareSerial Serial5(PD2, PC12);

constexpr auto baud_rate = 9600l;

constexpr auto status = obc::Status::Debug;

}  // namespace

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

    for (const auto &command : commands) {
        Serial5.println(command);
        delay(10000);
        String payload = Serial5.readString();
        if (payload.startsWith("ERR")) { return Err{Errc::Busy}; }
    }
    return Ok{Unit{}};
}

String lora_packet(const Packet &data)
{
    String to_send;
    to_send += String(data.position.altitude, 2) += String(";") +=
        String(data.position.latitude, 4) += String(";") +=
        String(data.position.longitude, 4) += String(";") +=
        String(data.position.speed / mph_to_kph_conversion) += String(";") +=
        String(data.bmp_measurements.temperature, 2) += String(";") +=
        String(to_underlying<Status>(status));

    return to_send;
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
    auto input = packet;
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
