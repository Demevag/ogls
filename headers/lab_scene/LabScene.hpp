#pragma once

#include "Scene.hpp"

#include "Engine.hpp"

namespace dmvg::lab {

class LabScene : public engine::Scene
{
    using super = engine::Scene;

public:
    LabScene()
        : super{{glm::vec3(4, 3, 8), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)},
                glm::perspective(glm::radians(engine::get_fov()),
                                 4.0f / 3.0f,
                                 0.1f,
                                 100.0f)}
    {}

    util::error init() override
    {
        auto result = engine::load_model("resource/models/backpack.obj",
                                  "resource/shaders/textured_shader.vert",
                                  "resource/shaders/textured_shader.frag");

        if (result.failed()) {
            return result.get_error();
        }

        m_backpack = std::move(result.get_value());
        add_object(m_backpack);
        
        result = engine::load_model("resource/models/gamecube-logo-cube.obj",
                                  "resource/shaders/textured_shader.vert",
                                  "resource/shaders/textured_shader.frag",
                                  glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.0f, 0.0f)));
        if (result.failed()) {
            return result.get_error();
        }

        add_object(std::move(result.get_value()));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        return std::nullopt;
    }

    void redraw() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_backpack->rotate({0.0f, 1.0f, 0.0f}, 0.01f);
        
        super::redraw();

    }
private:
    std::shared_ptr<engine::Object> m_backpack;
};

} // namespace dmvg::lab
