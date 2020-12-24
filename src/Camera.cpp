#include "Camera.hpp"

#include "Engine.hpp"

#include <chrono>

#include <glm/glm.hpp>

namespace dmvg::engine {

void Camera::go_to(glm::vec3 new_coords)
{
    m_coords = new_coords;
}

void Camera::look_at(glm::vec3 new_target)
{
    m_target = new_target;
}

void Camera::handle_inputs()
{
    static auto last_time = std::chrono::system_clock::now();

    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<float> delta_time = current_time - last_time;
    last_time = current_time;

    auto [horizontal_angle,
          vertical_angle] = handle_mouse_input(delta_time.count());

    m_horizontal_angle += horizontal_angle;
    m_vertical_angle += vertical_angle;

    glm::vec3 direction(cos(m_vertical_angle) * sin(m_horizontal_angle),
                        sin(m_vertical_angle),
                        cos(m_vertical_angle) * cos(m_horizontal_angle));

    // Right vector
    glm::vec3 right = glm::vec3(sin(m_horizontal_angle - 3.14159f / 2.0f),
                                0,
                                cos(m_horizontal_angle - 3.14159f / 2.0f));

    m_up_vector = glm::cross(right, direction);

    auto [move_direction, delta] = handle_keyboard_input(delta_time.count());

    auto forward_delta = direction * delta;
    auto right_delta = right * delta;
    switch (move_direction) {
        case MoveDirection::FORWARD:
            m_coords += forward_delta;
            break;
        case MoveDirection::BACK:
            m_coords -= forward_delta;
            break;
        case MoveDirection::RIGHT:
            m_coords += right_delta;
            break;
        case MoveDirection::LEFT:
            m_coords -= right_delta;
            break;
        default:
            break;
    }

    m_target = m_coords + direction;
}

} // namespace dmvg::engine
