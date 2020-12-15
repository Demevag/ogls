#include "Engine.hpp"
#include "Object.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace dmvg;
static const std::vector<GLfloat> g_vertex_buffer_data = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

int main()
{
    if (auto err = engine::init_screen(1024, 768, "Window name")) {
        std::cerr << *err << std::endl;
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    auto result = engine::load_model(g_vertex_buffer_data, "resource/shaders/vertex.shade", "resource/shaders/fragment.shade");    
    if (result.failed()) {
        std::cerr << result.get_error() << std::endl;
    }

    std::unique_ptr<engine::Object> triangle = std::move(result.get_value());

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        triangle->draw();

        engine::flush_buffers(); 
    } 
    while(engine::is_screen_opened());

    glDeleteVertexArrays(1, &VertexArrayID);

    return 0;
}
