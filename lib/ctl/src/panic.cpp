#include "ctl/panic.hpp"

#include <exception>

namespace ctl {

namespace {

inline PanicHandler handler = [](const char* msg, SourceLocation loc) {
    std::terminate();
};

}  // namespace

PanicHandler get_panic() { return handler; }

void set_panic(PanicHandler h) { handler = h; }

void panic(const char* msg, SourceLocation loc) { handler(msg, loc); }

}  // namespace ctl
