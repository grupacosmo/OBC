#ifndef OBC_LORA_HPP
#define OBC_LORA_HPP

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Base64.h>

#include "logger.hpp"
#include "result.hpp"

namespace obc {

Result<Unit, Errc> init_lora();
DynamicJsonDocument to_json(const Packet &);
String json_to_str(const DynamicJsonDocument &);
String encode(const String &);
void lora_serialize(const DynamicJsonDocument &);

}  // namespace obc

#endif