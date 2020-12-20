#include "Object.hpp"

namespace dmvg::engine {

class ObjectImpl : public Object
{
public:
    ObjectImpl(const GLuint & vertex_buffer_id, const GLuint & shader_program_id)
        : m_vertex_buffer_id{vertex_buffer_id},
          m_shader_program_id{shader_program_id}
    {
        m_matrix_id = glGetUniformLocation(m_shader_program_id, "mvp");
    }

    ~ObjectImpl()
    {
        glDeleteBuffers(1, &m_vertex_buffer_id);
        glDeleteProgram(m_shader_program_id);
    }

    void draw(const glm::mat4 & mvp) override
    {
        glUseProgram(m_shader_program_id);

        glUniformMatrix4fv(m_matrix_id, 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
        glVertexAttribPointer(
           0,                  
           3,                  // Size
           GL_FLOAT,           // Type
           GL_FALSE,           // Normalized?
           0,                  // Step
           (void*)0            // Buffer offset
        );

        glDrawArrays(GL_TRIANGLES, 0, 3); 

        glDisableVertexAttribArray(0);
    }

private:
    GLuint m_vertex_buffer_id;
    GLuint m_shader_program_id;
    GLuint m_matrix_id;
};

util::expected<std::unique_ptr<Object>> load_model(const fs::path & model, const fs::path & vertex_shader, const fs::path & frag_shader)
{
    return {"Not implemented yet"};
}

util::expected<std::unique_ptr<Object>> load_model(const std::vector<GLfloat> & model, const fs::path & vertex_shader, const fs::path & frag_shader)
{
    GLuint vertex_buffer_id;

    glGenBuffers(1, &vertex_buffer_id);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

    glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(GLfloat), model.data(), GL_STATIC_DRAW); 
        
    auto result = util::load_shaders(vertex_shader, frag_shader);
    if (result.failed()) {
        return {result.get_error()};
    }
    GLuint program_id = result.get_value();

    return {std::make_unique<ObjectImpl>(vertex_buffer_id, program_id)};
}

} // namespace dmvg::engine
