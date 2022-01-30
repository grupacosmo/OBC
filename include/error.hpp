#ifndef OBC_ERROR_HPP
#define OBC_ERROR_HPP

#include <Arduino.h>

#include "source_location.hpp"

namespace obc {

enum class Errc { Busy };

void panic(const char* msg, SourceLocation loc = SourceLocation::current());

}  // namespace obc

#endif
