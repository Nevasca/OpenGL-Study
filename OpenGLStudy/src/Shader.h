#pragma once
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:

    Shader(const std::string& FilePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    //Set uniforms
    void SetUniform1i(const std::string& Name, int Value);
    void SetUniform1iv(const std::string& Name, const int Count, const int* Value);
    void SetUniform1f(const std::string& Name, float Value);
    void SetUniform2f(const std::string& Name, const glm::vec2& Value);
    void SetUniform3f(const std::string& Name, float V0, float V1, float V2);
    void SetUniform3f(const std::string& Name, const glm::vec3& Value);
    void SetUniform4f(const std::string& Name, float V0, float V1, float V2, float V3);
    void SetUniform4f(const std::string& Name, const glm::vec4& Value);
    void SetUniformMat4f(const std::string& Name, const glm::mat4& mat);

private:

    unsigned int m_RendererID;
    std::string m_FilePath; // For debugging purpose

    // Hash table/map
    // mutable means this member can be edited inside a const method
    mutable std::unordered_map<std::string, int> m_UniformLocationCache; 

    ShaderProgramSource ParseShader(const std::string& FilePath);
    unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
    unsigned int CompileShader(unsigned int Type, const std::string& Source);
    int GetUniformLocation(const std::string& Name) const;
};
