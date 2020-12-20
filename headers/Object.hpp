#pragma once

#include "utils.hpp"

#include <filesystem>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

namespace dmvg::engine {

class Object
{
public:
    virtual ~Object() = default;

    virtual void draw(const glm::mat4 & mvp) = 0;
};

namespace fs = std::filesystem;
util::expected<std::unique_ptr<Object>> load_model(const fs::path & model, const fs::path & vertex_shader, const fs::path & frag_shader);

util::expected<std::unique_ptr<Object>> load_model(const std::vector<GLfloat> & model, const fs::path & vertex_shader, const fs::path & frag_shader);
} // namespace dmvg::engine
