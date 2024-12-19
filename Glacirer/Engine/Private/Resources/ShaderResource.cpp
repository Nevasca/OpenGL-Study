#include "Resources/ShaderResource.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "Rendering/Shader.h"

namespace Glacirer
{
    namespace Resources
    {
        std::shared_ptr<Rendering::Shader> ShaderResource::LoadShaderFromFile(const std::string& vertexShaderPath, const std::string& fragShaderPath)
        {
            Rendering::ShaderSource source{};

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

                source.VertexShader = vertexStream.str();
                source.FragmentShader = fragStream.str();
            }
            catch (const std::exception& e)
            {
                std::cout << "ERROR::SHADER: Failed to read shader files" << vertexShaderPath << " | " << fragShaderPath;
            }

            return std::make_shared<Rendering::Shader>(source);
        }

        std::shared_ptr<Rendering::Shader> ShaderResource::LoadShaderFromFile(const std::string& singleFileShaderPath)
        {
            Rendering::ShaderSource source{};
            std::regex uniformRegex(R"(uniform\s+(\w+)\s+(\w+);)");

            std::ifstream Stream(singleFileShaderPath);

            enum class EShaderType
            {
                None = -1,
                Vertex = 0,
                Fragment = 1,
                Geometry = 2
            };

            std::string line;
            std::stringstream ss[3]; // Vertex shader [0], Fragment [1] and Geometry [2]
            EShaderType type = EShaderType::None;

            while(getline(Stream, line))
            {
                if(line.find(SHADER_TYPE_KEYWORD) != std::string::npos)
                {
                    if(line.find(VERTEX_SHADER_KEYWORD) != std::string::npos)
                    {
                        type = EShaderType::Vertex;
                    }
                    else if(line.find(FRAGMENT_SHADER_KEYWORD) != std::string::npos)
                    {
                        type = EShaderType::Fragment;
                    }
                    else if(line.find(GEOMETRY_SHADER_KEYWORD) != std::string::npos)
                    {
                        type = EShaderType::Geometry;
                    }
                }
                else
                {
                    std::smatch match;
                    if(std::regex_match(line, match, uniformRegex))
                    {
                        std::string uniformType = match[1].str();
                        std::string uniformName = match[2].str();

                        if (uniformType == "sampler2D")
                        {
                            source.Properties.AddTexture(std::move(uniformName));
                        }
                        else if(uniformType == "vec4")
                        {
                            source.Properties.AddColor(std::move(uniformName));
                        }
                        else if(uniformType == "int" && uniformName != "u_RenderingMode")
                        {
                            source.Properties.AddInt(std::move(uniformName));
                        }
                        else if(uniformType == "float")
                        {
                            source.Properties.AddFloat(std::move(uniformName));
                        }
                    }
                    
                    ss[static_cast<int>(type)] << line << "\n";
                }
            }

            source.VertexShader = ss[0].str();
            source.FragmentShader = ss[1].str();
            source.GeometryShader = ss[2].str();
    
            return std::make_shared<Rendering::Shader>(source);
        }
    }
}
