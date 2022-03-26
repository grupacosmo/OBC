#ifndef OBC_LORA_HPP
#define OBC_LORA_HPP

#include "error.hpp"
#include "result.hpp"
#include "logger.hpp"

namespace obc{
Result<Unit, Errc> init_lora();
void lora_serialize(const Packet&);
}

#endif