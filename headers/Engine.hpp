#pragma once

#include "utils.hpp"

namespace dmvg::engine {

enum class MoveDirection
{
    FORWARD,
    BACK,
    RIGHT,
    LEFT,
    NONE
};

util::error init_screen(unsigned int width,
                        unsigned int height,
                        std::string name);

void set_move_speed(float move_speed);
void set_mouse_speed(float mouse_speed);
void set_fov(float fov);

float get_fov();

std::pair<float, float> handle_mouse_input(float delta_time);
std::pair<MoveDirection, float> handle_keyboard_input(float delta_time);

bool is_screen_opened();
void flush_buffers();

} // namespace dmvg::engine
