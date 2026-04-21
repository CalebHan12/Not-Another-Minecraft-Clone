#include "Shader.h"

#include <fstream>
#include <iostream>

Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
    loadShader(vertexPath, fragmentPath);
}

void Shader::loadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
        if(!std::filesystem::exists(vertexPath) || !std::filesystem::exists(fragmentPath)) {
        std::cout << "Shader file:" << vertexPath << " " << fragmentPath
            << " doesn't exist!" << std::endl;
        return;
    }

    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::exception e) {
        std::cout << "Shader not read properly: " << e.what() << std::endl;
        return;
    }
    const char* vShaderCode = vertexCode.c_str(), *fShaderCode = fragmentCode.c_str();

    // Vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, ShaderType::vertex);

    // Fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, ShaderType::fragment);

    // Shader program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    checkCompileErrors(id, ShaderType::program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const {
    glUseProgram(id);
}

void Shader::setBool(std::string_view name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.data()), (int)value);
}

void Shader::setInt(std::string_view name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.data()), value);
}

void Shader::setFloat(std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.data()), value);
}

void Shader::setVec2(std::string_view name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(id, name.data()), 1, &value[0]);
}

void Shader::setVec2(std::string_view name, float x, float y) const {
    glUniform2f(glGetUniformLocation(id, name.data()), x, y);
}

void Shader::setVec3(std::string_view name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(id, name.data()), 1, &value[0]);
}

void Shader::setVec3(std::string_view name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id, name.data()), x, y, z);
}

void Shader::setVec4(std::string_view name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(id, name.data()), 1, &value[0]);
}

void Shader::setVec4(std::string_view name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(id, name.data()), x, y, z, w);
}

void Shader::setMat2(std::string_view name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(std::string_view name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(std::string_view name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shader, ShaderType type) {
    int success;
    char infoLog[1024];
    if(type == ShaderType::vertex || type == ShaderType::fragment) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::string typeString = toString(type);
            std::cout << "Shader compile error: " << typeString << "\n" << infoLog << std::endl;
        }
    } else if(type == ShaderType::program) { // Shader program linking
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::string typeString = toString(type);
            std::cout << "Shader program linking error: " << typeString << "\n" << infoLog << std::endl;
        }
    }
}

std::string toString(ShaderType type) {
    switch (type) {
    case(ShaderType::vertex):
        return "Vertex";
    case(ShaderType::fragment):
        return "Fragment";
    case(ShaderType::program):
        return "Program";
    default:
        return "Unknown";
    }
}
