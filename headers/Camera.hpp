#pragma once

namespace dmvg::engine {

class Camera
{
public: 
    void go_to(glm::vec3 new_coords);
    void look_at(glm::vec3 new_target);

    void move(glm::vec3 delta);
    void turn_by(glm::vec3 delta);

private:
    glm::vec3 m_coords;
    glm::vec3 m_target;
    glm::vec3 m_up_vector;
};

} // namespace dmvg::engine
