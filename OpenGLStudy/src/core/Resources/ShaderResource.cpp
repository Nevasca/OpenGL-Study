#include "ShaderResource.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "core/Rendering/Shader.h"

std::shared_ptr<Shader> ShaderResource::LoadShaderFromFile(const std::string& vertexShaderPath, const std::string& fragShaderPath)
{
    std::string vertexSourceCode{};
    std::string fragSourceCode{};

    try
    {
        std::ifstream vertexShaderFile{vertexShaderPath};
        std::ifstream fragShaderFile{fragShaderPath};

        std::stringstream vertexStream;
        std::stringstream fragStream;

        vertexStream << vertexShaderFile.rdbuf();
        fragStream << fragShaderFile.rdbuf();

        vertexShaderFile.close();
        fragShaderFile.close();

        vertexSourceCode = vertexStream.str();
        fragSourceCode = fragStream.str();
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << vertexShaderPath << " | " << fragShaderPath;
    }

    return std::make_shared<Shader>(vertexSourceCode, fragSourceCode);
}

std::shared_ptr<Shader> ShaderResource::LoadShaderFromFile(const std::string& singleFileShaderPath)
{
    std::ifstream Stream(singleFileShaderPath);

    enum class EShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2]; // One for the vertex shader [0] and other for the fragment [1]
    EShaderType type = EShaderType::NONE;

    while(getline(Stream, line))
    {
        if(line.find(SHADER_TYPE_KEYWORD) != std::string::npos)
        {
            if(line.find(VERTEX_SHADER_KEYWORD) != std::string::npos)
            {
                type = EShaderType::VERTEX;
            }
            else
            {
                if(line.find(FRAGMENT_SHADER_KEYWORD) != std::string::npos)
                {
                    type = EShaderType::FRAGMENT;
                }
            }
        }
        else
        {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }

    return std::make_shared<Shader>(ss[0].str(), ss[1].str());
}
