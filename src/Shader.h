#pragma once
#include <cstdint>
#include <string>
#include <filesystem>

#include "glm/glm.hpp"
#include "glad/glad.h"

enum class ShaderType : uint8_t
{
    VERTEX = 0,
    FRAGMENT,
    PROGRAM
};

std::string ShaderTypeToString(ShaderType type);

class Shader {
public:
    Shader() = default;
    Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
    ~Shader() = default;

    Shader(const Shader& other) = default;
    Shader(Shader&& other) noexcept = default;

    Shader& operator=(const Shader& other) = default;
    Shader& operator=(Shader&& other) noexcept = default;

    void use();
    void setBool(std::string_view name, bool value) const;
    void setInt(std::string_view name, int value) const;
    void setFloat(std::string_view name, float value) const;

    void setVec2(std::string_view name, const glm::vec2& value) const;
    void setVec2(std::string_view name, float x, float y) const;

    void setVec3(std::string_view name, const glm::vec3& value) const;
    void setVec3(std::string_view name, float x, float y, float z) const;

    void setVec4(std::string_view name, const glm::vec4& value) const;
    void setVec4(std::string_view name, float x, float y, float z, float w) const;

    void setMat2(std::string_view name, const glm::mat2& mat) const;
    void setMat3(std::string_view name, const glm::mat3& mat) const;
    void setMat4(std::string_view name, const glm::mat4& mat) const;

private:
    GLuint id;

    static void checkCompileErrors(GLuint shader, ShaderType type);
};
