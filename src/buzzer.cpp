#include "buzzer.hpp"

#include <Arduino.h>

namespace obc {

void buzzer()
{
    digitalWrite(D6, 255);
    delay(100);
    digitalWrite(D6, LOW);
}

}  // namespace obc