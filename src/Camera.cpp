#include "Camera.hpp"

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

void Camera::move(glm::vec3 delta)
{
    //TODO: implement
}

void Camera::turn_by(glm::vec3 delta)
{
    //TODO: implement
}

} // namespace dmvg::engine
