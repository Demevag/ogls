#pragma once

#include "Scene.hpp"

#include "Engine.hpp"

namespace dmvg::lab {

class LabScene : public engine::Scene
{
    using super = engine::Scene;

public:
    LabScene()
        : super{{glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)},
                glm::perspective(glm::radians(engine::get_fov()),
                                 4.0f / 3.0f,
                                 0.1f,
                                 100.0f)}
    {}

    util::error init() override
    {
        auto result = engine::load_model("resource/models/bender.obj",
                                         "resource/shaders/vertex.shade",
                                         "resource/shaders/fragment.shade");

        if (result.failed()) {
            return result.get_error();
        }

        add_object(std::move(result.get_value()));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);

        glClearColor(0.0f, 0.0f, 0.6f, 0.0f);

        return std::nullopt;
    }

    void redraw() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        super::redraw();
    }
};

} // namespace dmvg::lab
