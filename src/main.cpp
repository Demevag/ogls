#include "Engine.hpp"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace dmvg;

int main()
{
    if (auto err = engine::init_screen(1024, 768, "Window name")) {
        std::cerr << *err << std::endl;
    }

    do{
       engine::flush_buffers(); 

    } 
    while(engine::is_screen_opened());

    return 0;
}
