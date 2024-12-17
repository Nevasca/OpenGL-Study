#include "Rendering/Shader.h"

#include "Rendering/OpenGLCore.h"

#include <iostream>
#include <fstream>
#include <string>

namespace Glacirer
{
    namespace Rendering
    {
        unsigned int Shader::m_LastBoundShaderId = 0;

        Shader::Shader(const ShaderSource& source)
            : m_Properties(source.Properties)
        {
            m_RendererID = CreateShader(source);
        }

        Shader::~Shader()
        {
            GLCall(glDeleteProgram(m_RendererID));

            // If next shader created uses the same id as the one destroyed and previously bound
            // it will think it's already bound and cause a OpenGL crash,
            // so it's safer to set last bound to none now (0)
            if(IsBound())
            {
                m_LastBoundShaderId = 0;
            }
        }

        void Shader::Bind() const
        {
            if(m_LastBoundShaderId != m_RendererID)
            {
                GLCall(glUseProgram(m_RendererID));
                m_LastBoundShaderId = m_RendererID;
            }
        }

        void Shader::Unbind() const
        {
            GLCall(glUseProgram(0));
            m_LastBoundShaderId = 0;
        }

        void Shader::SetUniform1i(const std::string& name, int value)
        {
            GLCall(glUniform1i(GetUniformLocation(name), value));
        }

        void Shader::SetUniform1iv(const std::string& name, const int count, const int* value)
        {
            GLCall(glUniform1iv(GetUniformLocation(name), count, value));
        }

        void Shader::SetUniform1f(const std::string& name, float value)
        {
            GLCall(glUniform1f(GetUniformLocation(name), value));
        }

        void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
        {
            GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
        }

        void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
        {
            GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
        }

        void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
        {
            SetUniform3f(name, value.x, value.y, value.z);
        }

        void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
        {
            GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
        }

        void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
        {
            GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
        }

        void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
        {
            GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
        }

        int Shader::GetUniformLocation(const std::string& name) const
        {
            if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            {
                return m_UniformLocationCache[name];
            }
    
            GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

#if ENABLE_SHADER_DEBUG
    if(location == -1)
    {
        std::cout << "Warning: Uniform " << name << " doesn't exist!\n";
    }
#endif

            m_UniformLocationCache[name] = location;

            return location;
        }

        unsigned int Shader::CreateShader(const Rendering::ShaderSource& source)
        {
            GLCall(unsigned int program = glCreateProgram());

            unsigned int Vs = CompileShader(GL_VERTEX_SHADER, source.VertexShader);
            unsigned int Fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentShader);

            GLCall(glAttachShader(program, Vs));
            GLCall(glAttachShader(program, Fs));
    
            unsigned int Gs = 0;
            bool bIsUsingGeometryShader = !source.GeometryShader.empty();

            if(bIsUsingGeometryShader)
            {
                Gs = CompileShader(GL_GEOMETRY_SHADER, source.GeometryShader);
                GLCall(glAttachShader(program, Gs));
            }

            GLCall(glLinkProgram(program));
            GLCall(glValidateProgram(program));

            GLCall(glDeleteShader(Vs));
            GLCall(glDeleteShader(Fs));

            if(bIsUsingGeometryShader)
            {
                GLCall(glDeleteShader(Gs));
            }

            return program;
        }

        unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
        {
            GLCall(unsigned int id = glCreateShader(type));
            const char* Src = source.c_str();

            GLCall(glShaderSource(id, 1, &Src, nullptr));
            GLCall(glCompileShader(id));

            int result;
            GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

            if(result == GL_FALSE)
            {
                int length;
                GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
                char* message = (char*)alloca(length * sizeof(char));
                GLCall(glGetShaderInfoLog(id, length, &length, message));

                std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
                std::cout << message << "\n";

                GLCall(glDeleteShader(id));

                return 0;
            }
    
            return id;
        }
    }
}
