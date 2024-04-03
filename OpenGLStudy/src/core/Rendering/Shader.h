#pragma once
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader
{
public:

    Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, const int value);
    void SetUniform1iv(const std::string& name, const int count, const int* value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform2f(const std::string& name, const glm::vec2& value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform3f(const std::string& name, const glm::vec3& value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform4f(const std::string& name, const glm::vec4& value);
    void SetUniformMat4f(const std::string& name, const glm::mat4& mat);

    unsigned int GetRendererID() const { return m_RendererID; }

private:

    static unsigned int m_LastBoundShaderId;
    
    unsigned int m_RendererID{0};
    mutable std::unordered_map<std::string, int> m_UniformLocationCache{};

    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    int GetUniformLocation(const std::string& name) const;
};
