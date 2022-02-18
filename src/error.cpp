#include "error.hpp"

#include <Arduino.h>

namespace obc {

void terminate_handler()
{
    while (true) {}
}

void panic_handler(const char* msg, ccl::SourceLocation loc)
{
    Serial.print("program panicked at '");
    Serial.print(msg);
    Serial.print("', ");
    Serial.print(loc.file());
    Serial.print(":");
    Serial.print(loc.function());
    Serial.print(":");
    Serial.print(loc.line());
    Serial.print("\n");
    Serial.flush();
    std::terminate();
}

}  // namespace obc
