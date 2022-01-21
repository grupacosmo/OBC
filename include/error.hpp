#ifndef OBC_ERROR_HPP
#define OBC_ERROR_HPP

#include <Arduino.h>

namespace obc {

enum class Errc { Busy };

#define OBC_PANIC(msg) \
    Serial.print(R"(program panicked at ")"); \
    Serial.print(msg); \
    Serial.print(R"(", )"); \
    Serial.print(__FILE__); \
    Serial.print(":"); \
    Serial.print(__LINE__); \
    Serial.print("\n"); \
    Serial.flush(); \
    while (true) {}

}  // namespace obc

#endif
