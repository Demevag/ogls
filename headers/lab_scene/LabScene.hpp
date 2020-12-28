#pragma once

#include "Scene.hpp"

#include "Engine.hpp"

namespace dmvg::lab {

static const std::vector<GLfloat> g_vertex_buffer_data =
{-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
 -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f,
 -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,
 -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,
 -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f,
 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,
 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,
 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f};

static const std::vector<GLfloat> g_uv_buffer_data =
{0.000059f, 1.0f - 0.000004f, 0.000103f, 1.0f - 0.336048f,
 0.335973f, 1.0f - 0.335903f, 1.000023f, 1.0f - 0.000013f,
 0.667979f, 1.0f - 0.335851f, 0.999958f, 1.0f - 0.336064f,
 0.667979f, 1.0f - 0.335851f, 0.336024f, 1.0f - 0.671877f,
 0.667969f, 1.0f - 0.671889f, 1.000023f, 1.0f - 0.000013f,
 0.668104f, 1.0f - 0.000013f, 0.667979f, 1.0f - 0.335851f,
 0.000059f, 1.0f - 0.000004f, 0.335973f, 1.0f - 0.335903f,
 0.336098f, 1.0f - 0.000071f, 0.667979f, 1.0f - 0.335851f,
 0.335973f, 1.0f - 0.335903f, 0.336024f, 1.0f - 0.671877f,
 1.000004f, 1.0f - 0.671847f, 0.999958f, 1.0f - 0.336064f,
 0.667979f, 1.0f - 0.335851f, 0.668104f, 1.0f - 0.000013f,
 0.335973f, 1.0f - 0.335903f, 0.667979f, 1.0f - 0.335851f,
 0.335973f, 1.0f - 0.335903f, 0.668104f, 1.0f - 0.000013f,
 0.336098f, 1.0f - 0.000071f, 0.000103f, 1.0f - 0.336048f,
 0.000004f, 1.0f - 0.671870f, 0.336024f, 1.0f - 0.671877f,
 0.000103f, 1.0f - 0.336048f, 0.336024f, 1.0f - 0.671877f,
 0.335973f, 1.0f - 0.335903f, 0.667969f, 1.0f - 0.671889f,
 1.000004f, 1.0f - 0.671847f, 0.667979f, 1.0f - 0.335851f};

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

        auto cube_ptr = std::move(result.get_value());
        /*if (auto err = cube_ptr
                       ->bind_texture("resource/textures/uvtemplate.tga",
                                      g_uv_buffer_data,
                                      "cube_texture")) {
            return err;
        }*/
        add_object(std::move(cube_ptr));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);

        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        return std::nullopt;
    }

    void redraw() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        super::redraw();
    }
};

} // namespace dmvg::lab
