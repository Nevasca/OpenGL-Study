#pragma once
#include <memory>
#include <string>

class Shader;

class ShaderResource
{
public:

    static std::shared_ptr<Shader> LoadShaderFromFile(const std::string& vertexShaderPath, const std::string& fragShaderPath);
    static std::shared_ptr<Shader> LoadShaderFromFile(const std::string& singleFileShaderPath);

private:

    static constexpr const char* SHADER_TYPE_KEYWORD = "#shader";
    static constexpr const char* VERTEX_SHADER_KEYWORD = "vertex";
    static constexpr const char* FRAGMENT_SHADER_KEYWORD = "fragment";
    static constexpr const char* GEOMETRY_SHADER_KEYWORD = "geometry";
};
