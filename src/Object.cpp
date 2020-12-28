#include "Object.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "assimp/DefaultLogger.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "libs/stb_image.h"

#include <iostream>

namespace dmvg::engine {

GLuint bind_vertexes_to_buffer(const vertexes & verts)
{
    GLuint buffer_id;

    glGenBuffers(1, &buffer_id);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

    glBufferData(GL_ARRAY_BUFFER,
                 verts.size() * sizeof(GLfloat),
                 verts.data(),
                 GL_STATIC_DRAW);

    return buffer_id;
}

void pass_buffer_to_shaders(std::size_t layout_num, GLuint buffer_id)
{
    glEnableVertexAttribArray(layout_num);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glVertexAttribPointer(layout_num,
                          3,        // Size
                          GL_FLOAT, // Type
                          GL_FALSE, // Normalized?
                          0,        // Step
                          (void *)0 // Buffer offset
    );
}

void pass_texture_to_shaders(std::size_t layout_num,
                             GLuint texture_id,
                             GLuint uniform_texture_id,
                             GLuint uv_buffer_id)
{
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(uniform_texture_id, 0);

    glEnableVertexAttribArray(layout_num);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
    glVertexAttribPointer(layout_num, // attribute. No particular reason for 1,
                                      // but must match the layout in the
                                      // shader.
                          2,          // size : U+V => 2
                          GL_FLOAT,   // type
                          GL_FALSE,   // normalized?
                          0,          // stride
                          (void *)0   // array buffer offset
    );
}

struct Mesh
{
    vertexes m_vertexes;
    std::optional<vertexes> m_colors;
    std::optional<vertexes> m_uv;
    std::optional<vertexes> m_normals;
};

struct MeshInfo
{
    struct TextureInfo
    {
        GLuint m_uv_buffer_id;
        GLuint m_texture_id;
        GLuint m_uniform_texture_id;
    };

    size_t m_vertexes_num;
    GLuint m_vertex_buffer_id;
    std::optional<GLuint> m_colors_buffer_id;
    std::optional<TextureInfo> m_texture;
    std::optional<GLuint> m_normals_buffer_id;
};

void draw_mesh(const MeshInfo & mesh_info,
               const glm::mat4 & mvp,
               const GLuint & shader_program_id,
               const GLuint & matrix_id)
{
    glUseProgram(shader_program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

    pass_buffer_to_shaders(0, mesh_info.m_vertex_buffer_id);
    if (mesh_info.m_texture) {
        const auto & texture_info = *mesh_info.m_texture;
        pass_texture_to_shaders(1,
                                texture_info.m_texture_id,
                                texture_info.m_uniform_texture_id,
                                texture_info.m_uv_buffer_id);
    }
    if (mesh_info.m_colors_buffer_id) {
        pass_buffer_to_shaders(1, *mesh_info.m_colors_buffer_id);
    }

    glDrawArrays(GL_TRIANGLES, 0, mesh_info.m_vertexes_num);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

util::expected<MeshInfo::TextureInfo> bind_texture_to_mesh(
    fs::path filename,
    std::string name,
    const vertexes & uv_coords,
    GLuint shader_program_id)
{
    int width, height, format;
    stbi_set_flip_vertically_on_load(1);
    unsigned char * image = stbi_load(filename.c_str(),
                                      &width,
                                      &height,
                                      &format,
                                      STBI_rgb);

    if (image == nullptr)
        return {"Failed to load texture'" + filename.string() + "'"};

    GLuint texture_id;
    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 width,
                 height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 image);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);

    GLuint uniform_texture_id = glGetUniformLocation(shader_program_id,
                                                     name.c_str());

    GLuint uv_buffer_id = bind_vertexes_to_buffer(uv_coords);
    return {
        MeshInfo::TextureInfo{uv_buffer_id, texture_id, uniform_texture_id}};
}

class SimpleObjectImpl : public Object
{
public:
    SimpleObjectImpl(Mesh && mesh,
                     MeshInfo mesh_info,
                     const GLuint & shader_program_id)
        : m_mesh{mesh},
          m_mesh_info{mesh_info},
          m_shader_program_id{shader_program_id}
    {
        m_matrix_id = glGetUniformLocation(m_shader_program_id, "mvp");
    }

    ~SimpleObjectImpl() { glDeleteProgram(m_shader_program_id); }

    util::error bind_texture(fs::path filename,
                             vertexes && uv_coords,
                             std::string name) override
    {
        m_mesh.m_uv = std::move(uv_coords);

        auto result = bind_texture_to_mesh(filename,
                                           name,
                                           *m_mesh.m_uv,
                                           m_shader_program_id);
        if (result.failed()) {
            return {result.get_error()};
        }
        m_mesh_info.m_texture = result.get_value();

        return std::nullopt;
    }

    void draw(const glm::mat4 & mvp) override
    {
        draw_mesh(m_mesh_info, mvp, m_shader_program_id, m_matrix_id);
    }

private:
    Mesh m_mesh;
    MeshInfo m_mesh_info;

    GLuint m_shader_program_id;
    GLuint m_matrix_id;
};

class ComplexObjectImpl : public Object
{
public:
    ComplexObjectImpl(std::vector<Mesh> && meshes,
                      std::vector<MeshInfo> && meshes_info,
                      const GLuint & shader_program_id)
        : m_meshes{std::move(meshes)},
          m_meshes_info{std::move(meshes_info)},
          m_shader_program_id{shader_program_id}
    {
        m_matrix_id = glGetUniformLocation(m_shader_program_id, "mvp");
    }

    ~ComplexObjectImpl() { glDeleteProgram(m_shader_program_id); }

    util::error bind_texture(fs::path filename,
                             vertexes && uv_coords,
                             std::string name) override
    {
        return "Cannot bind texture to multimesh object";
    }

    void draw(const glm::mat4 & mvp) override
    {
        for (auto & mesh_info : m_meshes_info) {
            draw_mesh(mesh_info, mvp, m_shader_program_id, m_matrix_id);
        }
    }

private:
    std::vector<Mesh> m_meshes;
    std::vector<MeshInfo> m_meshes_info;

    GLuint m_shader_program_id;
    GLuint m_matrix_id;
};

util::expected<std::unique_ptr<Object>> load_model(
    const fs::path & model_file,
    const fs::path & vertex_shader,
    const fs::path & frag_shader)
{
    auto result = util::load_shaders(vertex_shader, frag_shader);
    if (result.failed()) {
        return {result.get_error()};
    }

    GLuint program_id = result.get_value();

    Assimp::DefaultLogger::create("logs", Assimp::Logger::VERBOSE);
    Assimp::Importer importer;
    const auto * scene = importer.ReadFile(model_file.string(),
                                           aiProcess_JoinIdenticalVertices |
                                               aiProcess_Triangulate |
                                               aiProcess_FixInfacingNormals);

    std::vector<Mesh> meshes;
    std::vector<MeshInfo> meshes_info;
    meshes.reserve(scene->mNumMeshes);
    for (size_t mesh_num = 0; mesh_num < scene->mNumMeshes; mesh_num++) {
        Mesh mesh;
        auto * mesh_data = scene->mMeshes[mesh_num];

        for (size_t i = 0; i < mesh_data->mNumFaces; i++) {
            const auto & face = mesh_data->mFaces[i];

            for (size_t j = 0; j < face.mNumIndices; j++) {
                if (mesh_data->mTextureCoords[0] != nullptr) {
                    if (!mesh.m_uv) {
                        mesh.m_uv = {};
                    }
                    auto uv_input = mesh_data
                                        ->mTextureCoords[0][face.mIndices[j]];
                    mesh.m_uv->insert(mesh.m_uv->end(),
                                      &uv_input[0],
                                      &uv_input[0] + 2);
                } else if (mesh_data->mColors[0] != nullptr) {
                    if (!mesh.m_colors) {
                        mesh.m_colors = {};
                    }
                    auto color_input = mesh_data->mColors[0][face.mIndices[j]];
                    mesh.m_colors->insert(mesh.m_colors->end(),
                                          &color_input[0],
                                          &color_input[0] + 3);
                }

                auto vert_input = mesh_data->mVertices[face.mIndices[j]];
                mesh.m_vertexes.insert(mesh.m_vertexes.end(),
                                       &vert_input[0],
                                       &vert_input[0] + 3);
            }
        }

        MeshInfo mesh_info{
            .m_vertexes_num = mesh.m_vertexes.size(),
            .m_vertex_buffer_id = bind_vertexes_to_buffer(mesh.m_vertexes),
        };

        if (mesh.m_colors) {
            mesh_info.m_colors_buffer_id = bind_vertexes_to_buffer(
                *mesh.m_colors);
        }
        if (mesh.m_uv) {
            MeshInfo::TextureInfo texture_info{
                .m_uv_buffer_id = bind_vertexes_to_buffer(*mesh.m_uv)};
            mesh_info.m_texture = texture_info;
        }
        meshes.push_back(std::move(mesh));
        meshes_info.push_back(mesh_info);
    }

    Assimp::DefaultLogger::kill();
    return {std::make_unique<ComplexObjectImpl>(std::move(meshes),
                                                std::move(meshes_info),
                                                program_id)};
}

util::expected<std::unique_ptr<Object>> load_model(
    const vertexes & model,
    const fs::path & vertex_shader,
    const fs::path & frag_shader)
{
    auto result = util::load_shaders(vertex_shader, frag_shader);
    if (result.failed()) {
        return {result.get_error()};
    }
    GLuint program_id = result.get_value();

    Mesh mesh = {.m_vertexes = model};

    MeshInfo mesh_info{.m_vertexes_num = mesh.m_vertexes.size(),
                       .m_vertex_buffer_id = bind_vertexes_to_buffer(
                           mesh.m_vertexes)};

    return {std::make_unique<SimpleObjectImpl>(std::move(mesh),
                                               std::move(mesh_info),
                                               program_id)};
}

util::expected<std::unique_ptr<Object>> load_model(
    const vertexes & model,
    const vertexes & colors,
    const fs::path & vertex_shader,
    const fs::path & frag_shader)
{
    auto result = util::load_shaders(vertex_shader, frag_shader);
    if (result.failed()) {
        return {result.get_error()};
    }
    GLuint program_id = result.get_value();

    Mesh mesh = {.m_vertexes = model, .m_colors = colors};

    MeshInfo mesh_info{.m_vertexes_num = mesh.m_vertexes.size(),
                       .m_vertex_buffer_id = bind_vertexes_to_buffer(
                           mesh.m_vertexes),
                       .m_colors_buffer_id = bind_vertexes_to_buffer(
                           *mesh.m_colors)};

    return {std::make_unique<SimpleObjectImpl>(std::move(mesh),
                                               std::move(mesh_info),
                                               program_id)};
}

} // namespace dmvg::engine
