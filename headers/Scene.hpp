#pragma once

#include "Camera.hpp"
#include "Object.hpp"
#include "utils.hpp"

#include <list>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dmvg::engine {

class Scene
{
public:
    Scene(Camera camera, glm::mat4 scene_projection)
        : m_camera{camera},
          m_scene_projection{scene_projection}
    {}
    virtual ~Scene() = default;

    virtual util::error init() = 0;
    void add_object(std::shared_ptr<Object> obj);
    virtual void redraw();

private:
    std::list<std::shared_ptr<Object>> m_objects;
    Camera m_camera;
    glm::mat4 m_scene_projection;
};

} // namespace dmvg::engine
