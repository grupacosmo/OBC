#include "buzzer.hpp"

#include <Arduino.h>

namespace obc {

void buzzer(std::size_t quantity)
{
    for (std::size_t i = 0; i < quantity; ++i) {
        digitalWrite(D6, HIGH);
        delay(100);
        digitalWrite(D6, LOW);
        delay(200);
    }
}

}  // namespace obc