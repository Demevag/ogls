#include "Object.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

namespace dmvg::engine {

GLuint bind_vertexes_to_buffer(const vertexes & verts)
{
    GLuint buffer_id;

    glGenBuffers(1, &buffer_id);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW);

    return buffer_id;
}

void pass_buffer_to_shaders(std::size_t layout_num, GLuint buffer_id)
{
    glEnableVertexAttribArray(layout_num);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glVertexAttribPointer(
        layout_num,                  
        3,                  // Size
        GL_FLOAT,           // Type
        GL_FALSE,           // Normalized?
        0,                  // Step
        (void*)0            // Buffer offset
    );

}

void pass_texture_to_shaders(std::size_t layout_num, GLuint texture_id, GLuint uniform_texture_id, GLuint uv_buffer_id)
{
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(uniform_texture_id, 0);

    glEnableVertexAttribArray(layout_num);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
}

class ObjectImpl : public Object
{
public:
    ObjectImpl(vertexes model, const GLuint & shader_program_id)
        : m_vertexes{model},
          m_colors{{}},
          m_shader_program_id{shader_program_id}
    {
        m_vert_buffer_id = bind_vertexes_to_buffer(m_vertexes);
        m_matrix_id = glGetUniformLocation(m_shader_program_id, "mvp");

        m_use_texture = false;
    }
    
    ObjectImpl(vertexes model, vertexes colors, const GLuint & shader_program_id)
        : m_vertexes{model},
          m_colors{colors},
          m_shader_program_id{shader_program_id}
    {
        m_vert_buffer_id = bind_vertexes_to_buffer(m_vertexes);
        m_colors_buffer_id = bind_vertexes_to_buffer(m_colors);
        m_matrix_id = glGetUniformLocation(m_shader_program_id, "mvp");

        m_use_texture = false;
    }

    ~ObjectImpl()
    {
        glDeleteBuffers(1, &m_vert_buffer_id);
        glDeleteBuffers(1, &m_colors_buffer_id);
        glDeleteProgram(m_shader_program_id);
    }

    util::error bind_texture(fs::path filename, vertexes uv_coords, std::string name) override
    {
        int width, height, format;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* image = stbi_load(filename.c_str(), &width, &height, &format, STBI_rgb);

        if(image == nullptr)
            return "Failed to load texture'" + filename.string() + "'";

        glGenTextures(1, &m_texture_id);

        glBindTexture(GL_TEXTURE_2D, m_texture_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(image); 
        
        m_uniform_texture_id = glGetUniformLocation(m_shader_program_id, name.c_str());

        m_colors = uv_coords;
        m_colors_buffer_id = bind_vertexes_to_buffer(m_colors);        

        m_use_texture = true;

        return std::nullopt; 
    }

    void draw(const glm::mat4 & mvp) override
    {
        glUseProgram(m_shader_program_id);

        glUniformMatrix4fv(m_matrix_id, 1, GL_FALSE, &mvp[0][0]);
        
        pass_buffer_to_shaders(0, m_vert_buffer_id);
        if (m_use_texture) {
            pass_texture_to_shaders(1, m_texture_id, m_uniform_texture_id, m_colors_buffer_id);
        }
        else {
            pass_buffer_to_shaders(1, m_colors_buffer_id);
        }
        
        glDrawArrays(GL_TRIANGLES, 0, m_vertexes.size()); 

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

private:
    vertexes m_vertexes;
    vertexes m_colors;

    GLuint m_vert_buffer_id;
    GLuint m_colors_buffer_id;

    bool m_use_texture;
    GLuint m_texture_id;
    GLuint m_uniform_texture_id;

    GLuint m_shader_program_id;
    GLuint m_matrix_id;

    
};

util::expected<std::unique_ptr<Object>> load_model(const fs::path & model, const fs::path & vertex_shader, const fs::path & frag_shader)
{
    return {"Not implemented yet"};
}

util::expected<std::unique_ptr<Object>> load_model(const vertexes & model, const fs::path & vertex_shader, const fs::path & frag_shader)
{
    auto result = util::load_shaders(vertex_shader, frag_shader);
    if (result.failed()) {
        return {result.get_error()};
    }
    GLuint program_id = result.get_value();

    return {std::make_unique<ObjectImpl>(model, program_id)};
}

util::expected<std::unique_ptr<Object>> load_model(const vertexes & model, const vertexes & colors, const fs::path & vertex_shader, const fs::path & frag_shader)
{
    auto result = util::load_shaders(vertex_shader, frag_shader);
    if (result.failed()) {
        return {result.get_error()};
    }
    GLuint program_id = result.get_value();

    return {std::make_unique<ObjectImpl>(model, colors, program_id)};
}

} // namespace dmvg::engine
