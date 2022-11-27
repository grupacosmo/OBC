#include "error.hpp"

#include "buzzer.hpp"

namespace obc {

namespace {

constexpr std::size_t buzzer_ind_panic = 3;

}  // namespace

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
    while (true) { obc::buzzer(buzzer_ind_panic); }
}

}  // namespace obc
