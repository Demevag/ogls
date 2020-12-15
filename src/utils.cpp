#include "utils.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>

namespace dmvg::util {

namespace {

error load_code(const std::filesystem::path & shader_file, std::ostream & sout)
{
    std::ifstream shader_stream(shader_file, std::ios::in);
    if(!shader_stream.is_open())
    {
        return "Failed to open shader file: " + shader_file.string();
    }
    
    sout << shader_stream.rdbuf();
    shader_stream.close();
        
    return std::nullopt;
}

error compile(std::string code, const GLuint & shader_id)
{
    const char * code_ptr = code.c_str();
    glShaderSource(shader_id, 1, &code_ptr , nullptr);
    glCompileShader(shader_id);

    // Выполняем проверку Вершинного шейдера
    GLint result = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

    int info_log_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

    if ( info_log_length > 0 ){
        std::vector<char> error(info_log_length+1);
        glGetShaderInfoLog(shader_id, info_log_length, nullptr, &error[0]);

        return std::string{error.begin(), error.end()};
    }

    return std::nullopt;
}

} // unnamed namespace

expected<GLuint> load_shaders(const std::filesystem::path & vertex_file, const std::filesystem::path & fragment_file)
{
    // Создаем шейдеры
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Загружаем код Вершинного Шейдера из файла
    std::ostringstream vertex_shader_code;
    if (auto error = load_code(vertex_file, vertex_shader_code)) {
        return {*error};
    }
    

    // Загружаем код Фрагментного шейдера из файла
    std::ostringstream fragment_shader_code;
    if (auto error = load_code(fragment_file, fragment_shader_code)) {
        return {*error};
    }
    

    // Компилируем Вершинный шейдер
    if (auto error = compile(vertex_shader_code.str(), vertex_shader_id)) {
        return {*error};
    } 

    // Компилируем Фрагментный шейдер
    if (auto error = compile(fragment_shader_code.str(), fragment_shader_id)) {
        return {*error};
    } 

    // Создаем шейдерную программу и привязываем шейдеры к ней
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    // Проверяем шейдерную программу
    GLint result = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);

    int info_log_length;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if ( info_log_length > 0 ){
      std::vector<char> error(info_log_length+1);
      glGetProgramInfoLog(program_id, info_log_length, nullptr, &error[0]);

      return {std::string{error.begin(), error.end()}};
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return {program_id};
}

} // namespace dmvg::util
