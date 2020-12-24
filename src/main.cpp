#include "Engine.hpp"
#include "Object.hpp"

#include "lab_scene/LabScene.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace dmvg;

int main()
{
    if (auto err = engine::init_screen(1024, 768, "Window name")) {
        std::cerr << *err << std::endl;
        return -1;
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    lab::LabScene scene;
    if (auto err = scene.init()) {
        std::cerr << *err << std::endl;
        return -1;
    }

    do {
        scene.redraw();
        engine::flush_buffers();
    } while (engine::is_screen_opened());

    glDeleteVertexArrays(1, &VertexArrayID);

    return 0;
}
