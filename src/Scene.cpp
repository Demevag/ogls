#include "Scene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dmvg::engine {

void Scene::add_object(std::shared_ptr<Object> obj)
{
    m_objects.push_back(obj);
}

void Scene::redraw()
{
    m_camera.handle_inputs();

    glm::mat4 view = glm::lookAt(m_camera.get_coords(),
                                 m_camera.get_target(),
                                 m_camera.get_up_vector());

    // As model matrix is constant now, mvp matrix will be same for all objects
    glm::mat4 vp = m_scene_projection * view;
    for (const auto & obj : m_objects) {
        obj->draw(vp, m_camera.get_coords());
    }
}

} // namespace dmvg::engine
