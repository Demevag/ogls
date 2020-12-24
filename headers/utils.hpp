#pragma once

#include <filesystem>
#include <optional>
#include <string_view>
#include <variant>

#include <GL/glew.h>

namespace dmvg::util {

using error = std::optional<std::string>;

template<typename OutT>
struct expected
{
    std::variant<OutT, std::string> m_value;

    bool failed() { return std::holds_alternative<std::string>(m_value); }

    bool succeeded() { return std::holds_alternative<OutT>(m_value); }

    std::string & get_error() { return std::get<std::string>(m_value); }

    OutT & get_value() { return std::get<OutT>(m_value); }
};

expected<GLuint> load_shaders(const std::filesystem::path & vertex_file,
                              const std::filesystem::path & fragment_file);

} // namespace dmvg::util
