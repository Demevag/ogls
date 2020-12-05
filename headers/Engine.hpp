#pragma once

#include "utils.hpp"

namespace dmvg::engine {

util::error init_screen(unsigned int width, unsigned int height, std::string name);

bool is_screen_opened();
void flush_buffers();

class Engine
{

};

} // namespace dmvg::engine
