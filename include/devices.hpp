#ifndef OBC_DEVICES_HPP
#define OBC_DEVICES_HPP

#include <Arduino.h>
#include <SD.h>

#include "acceleration.hpp"
#include "barometer.hpp"
#include "gps.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace obc {

void init();

}

#endif