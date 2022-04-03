#ifndef OBC_LORA_HPP
#define OBC_LORA_HPP

#include <Arduino.h>
#include <Base64.h>

#include "logger.hpp"
#include "result.hpp"

namespace obc {

Result<Unit, Errc> init_lora();
String lora_packet(const Packet &);
String make_string_from_count(size_t count, char c);
String encode(const String &);
void send_packet(const String &);

}  // namespace obc

#endif