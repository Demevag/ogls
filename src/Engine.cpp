#include "Engine.hpp"

#include "utils.hpp"

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace dmvg::engine {

struct ControlsSettings
{
    float move_speed = 3.0f;
    float mouse_speed = 0.005f;
    float fov = 45.0f;
};

GLFWwindow * window;
static ControlsSettings controls_settings;

util::error init_screen(unsigned int width,
                        unsigned int height,
                        std::string name)
{
    if (!glfwInit()) {
        return "GLFW init failed.";
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width,
                              height,
                              name.c_str(),
                              glfwGetPrimaryMonitor() /*monitor for fullscreen*/,
                              nullptr /*monitor to share*/);
    if (!window) {
        glfwTerminate();
        return "Cannot open window.";
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        return "Cannot initialize Glew.";
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return std::nullopt;
}

void set_move_speed(float move_speed)
{
    controls_settings.move_speed = move_speed;
}

void set_mouse_speed(float mouse_speed)
{
    controls_settings.mouse_speed = mouse_speed;
}

void set_fov(float fov)
{
    controls_settings.fov = fov;
}

float get_fov()
{
    return controls_settings.fov;
}

std::pair<float, float> handle_mouse_input(float delta_time)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glfwSetCursorPos(window, width / 2, height / 2);

    float horizontal_angle = controls_settings.mouse_speed * delta_time *
                             float(width / 2 - xpos);
    float vertical_angle = controls_settings.mouse_speed * delta_time *
                           float(height / 2 - ypos);

    return {horizontal_angle, vertical_angle};
}

std::pair<MoveDirection, float> handle_keyboard_input(float delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        return {MoveDirection::FORWARD,
                delta_time * controls_settings.move_speed};
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        return {MoveDirection::BACK, delta_time * controls_settings.move_speed};
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        return {MoveDirection::RIGHT,
                delta_time * controls_settings.move_speed};
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        return {MoveDirection::LEFT, delta_time * controls_settings.move_speed};
    }

    return {MoveDirection::NONE, 0.0};
}

bool is_screen_opened()
{
    return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0;
}

void flush_buffers()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

} // namespace dmvg::engine
