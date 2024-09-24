#pragma once
#include <map>
#include <memory>

class Shader;

namespace Rendering
{
    struct ActiveShader
    {
        std::shared_ptr<Shader> Shader{};
        unsigned int UsageCount{0};
    };
    
    class ShaderRenderSet
    {
    public:

        void Add(const std::shared_ptr<Shader>& shader);
        void Remove(const std::shared_ptr<Shader>& shader);
        bool Contains(const std::shared_ptr<Shader>& shader);
        void Clear();

        std::map<unsigned int, ActiveShader>& GetShaders() { return m_UniqueActiveShaders; }

    private:
        std::map<unsigned int, ActiveShader> m_UniqueActiveShaders{}; // Keyed by shader id
    };
}
