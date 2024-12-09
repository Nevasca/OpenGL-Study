#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#define ENABLE_SHADER_DEBUG 0

namespace Glacirer
{
    namespace Rendering
    {
        struct ShaderSource
        {
            std::string VertexShader{};
            std::string FragmentShader{};
            std::string GeometryShader{};
        };

        class Shader
        {
        public:

            Shader(const ShaderSource& source);
            ~Shader();

            void Bind() const;
            void Unbind() const;
            bool IsBound() const { return m_LastBoundShaderId == m_RendererID; }

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
            void SetName(const std::string& name) { m_Name = name; }
            std::string GetName() const { return m_Name; }

        private:

            static unsigned int m_LastBoundShaderId;
    
            unsigned int m_RendererID{0};
            mutable std::unordered_map<std::string, int> m_UniformLocationCache{};
            std::string m_Name{};

            unsigned int CreateShader(const ShaderSource& source);
            unsigned int CompileShader(unsigned int type, const std::string& source);
            int GetUniformLocation(const std::string& name) const;
        };
    }
}
