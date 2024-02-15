#pragma once
#include <string>
#include <unordered_map>

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
    void SetUniform1f(const std::string& Name, float Value);
    void SetUniform4f(const std::string& Name, float V0, float V1, float V2, float V3);

private:

    unsigned int m_RendererID;
    std::string m_FilePath; // For debugging purpose
    std::unordered_map<std::string, int> m_UniformLocationCache; // Hash table/map

    ShaderProgramSource ParseShader(const std::string& FilePath);
    unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
    unsigned int CompileShader(unsigned int Type, const std::string& Source);
    int GetUniformLocation(const std::string& Name);
};
