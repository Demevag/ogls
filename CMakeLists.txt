cmake_minimum_required(VERSION 3.16)

project(OpenglStudy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall")

set(SOURCES src/main.cpp src/Engine.cpp src/Camera.cpp src/Object.cpp src/Scene.cpp src/Shader.cpp)

add_executable(OpenglStudy ${SOURCES})

include_directories("headers/")

find_library(PTHREAD_LIB pthread)
find_library(GLFW_LIB glfw)
find_library(GLEW_LIB GLEW)
find_library(GL_LIB GL)
find_library(ASSIMP_LIB assimp)

target_link_libraries(OpenglStudy LINK_PUBLIC ${PTHREAD_LIB} ${GLFW_LIB} ${GLEW_LIB} ${GL_LIB} ${ASSIMP_LIB})

