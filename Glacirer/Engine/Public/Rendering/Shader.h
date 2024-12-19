#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#define ENABLE_SHADER_DEBUG 0

namespace Glacirer
{
    namespace Rendering
    {
        struct ShaderProperties
        {
            std::vector<std::string> Textures{};
            std::vector<std::string> Colors{};
            std::vector<std::string> Integers{};
            std::vector<std::string> Floats{};

            void AddTexture(const std::string&& uniformName) { Textures.emplace_back(uniformName); }
            void AddColor(const std::string&& uniformName) { Colors.emplace_back(uniformName); }
            void AddInt(const std::string&& uniformName) { Integers.emplace_back(uniformName); }
            void AddFloat(const std::string&& uniformName) { Floats.emplace_back(uniformName); }
        };
        
        struct ShaderSource
        {
            std::string VertexShader{};
            std::string FragmentShader{};
            std::string GeometryShader{};

            ShaderProperties Properties{};
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
            const ShaderProperties& GetProperties() const { return m_Properties; }

        private:

            static unsigned int m_LastBoundShaderId;
    
            unsigned int m_RendererID{0};
            mutable std::unordered_map<std::string, int> m_UniformLocationCache{};
            std::string m_Name{};
            ShaderProperties m_Properties{};

            unsigned int CreateShader(const ShaderSource& source);
            unsigned int CompileShader(unsigned int type, const std::string& source);
            int GetUniformLocation(const std::string& name) const;
        };
    }
}
