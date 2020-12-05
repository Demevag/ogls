#include "Engine.hpp"

#include "utils.hpp"

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace dmvg::engine {

GLFWwindow * window;

util::error init_screen(unsigned int width, unsigned int height, std::string name)
{
    if( !glfwInit() )
    {
        return "GLFW init failed.";
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( width, height, name.c_str(), nullptr /*monitor for fullscreen*/, nullptr /*monitor to share*/);
    if(!window ){
    	glfwTerminate();
        return "Cannot open window.";
    }
    glfwMakeContextCurrent(window);

    glewExperimental=true;
    if (glewInit() != GLEW_OK) {
        return "Cannot initialize Glew.";
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return std::nullopt;
}

bool is_screen_opened()
{
    return glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
}

void flush_buffers()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

} // namespace dmvg::engine
