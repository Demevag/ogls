#pragma once

#include <glm/glm.hpp>

namespace dmvg::engine {

class Camera
{
public: 
    Camera(glm::vec3 coords, glm::vec3 target, glm::vec3 up_vector)
        : m_coords{coords},
          m_target{target},
          m_up_vector{up_vector}
    {}

    void go_to(glm::vec3 new_coords);
    void look_at(glm::vec3 new_target);

    void move(glm::vec3 delta);
    void turn_by(glm::vec3 delta);

    const glm::vec3 & get_coords() { return m_coords; }
    const glm::vec3 & get_target() { return m_target; }
    const glm::vec3 & get_up_vector() { return m_up_vector; }
private:
    glm::vec3 m_coords;
    glm::vec3 m_target;
    glm::vec3 m_up_vector;
};

} // namespace dmvg::engine
