#include "ctl/panic.hpp"

namespace ctl {

void panic(const char* msg, SourceLocation loc)
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
    while (true) {}
}

}  // namespace ctl
