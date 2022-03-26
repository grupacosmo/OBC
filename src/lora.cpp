#include "lora.hpp"
#include <ArduinoJson.h>
#include <Base64.h>
#include <Arduino.h>
#include "result.hpp"


namespace obc{

HardwareSerial Serial5(PD2, PC12);

Result<Unit, Errc> init_lora()
{
    constexpr auto baud_rate = 9600l;
    constexpr std::array arr2 = {"AT+DR=EU868", "AT+CH=NUM,0-2", "AT+MODE=LWOTAA","AT+JOIN","AT+MSG=\"e2pzb246IFt7ImFvcHFyIjoiMjAuMyIsfV19Cg==\""};
    Serial5.begin(baud_rate);

    for (const auto elem : arr2) {
        Serial5.println(elem);
        delay(10000);
        String payload = Serial5.readString();
        if (payload.startsWith("ERR")) {
            return Err{Errc::Busy};
        }
    }

    return Ok{Unit{}};
}

void lora_serialize(const Packet& data){
    constexpr auto baud_rate = 9600l;
    Serial.begin(baud_rate);
    //Serial5.begin(a)
    Serial.println("gds");

    StaticJsonDocument<10> json;

    //DynamicJsonDocument json(1024);
    json["AccX"] = data.acclr_measurements.x;
    json["AccY"] = data.acclr_measurements.y;
    json["AccZ"] = data.acclr_measurements.z;
    json["Altidute"] = data.bmp_measurements.altitude;
    json["Fix"] = data.position.fix;
    json["Pressure"] = data.bmp_measurements.pressure;
    json["Location"] = String(data.position.latitude) + String(data.position.longitude);
    json["Satelites"] = data.position.satelites;
    json["Speed"] = data.position.speed / mph_to_kph_conversion;
    json["Temp"] = data.bmp_measurements.temperature;
    json["Time"] = String(data.date.year)+ String(data.date.month)+ String(data.date.day)+ 
    String(data.time.hour) + String(data.time.minute) + String(data.time.seconds) + String(data.time.milliseconds);

    String input = json.as<String>();
    char inputString[100];
    input.toCharArray(inputString, 100);
    int inputStringLength = sizeof(inputString);
    char encodedString[100];
    Base64.encode(encodedString, inputString, inputStringLength);
    Serial.println(encodedString);
    String encoded = "AT+MSG=\"" + String(encodedString) +"\"";
    Serial5.println(encoded);
    Serial.println(encoded);
}
}
