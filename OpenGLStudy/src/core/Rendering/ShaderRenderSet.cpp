#include "ShaderRenderSet.h"
#include "Shader.h"

namespace Rendering
{
    void ShaderRenderSet::Add(const std::shared_ptr<Shader>& shader)
    {
        assert(shader);

        const unsigned int shaderId = shader->GetRendererID();
        
        if(!Contains(shader))
        {
            ActiveShader activeShader{};
            activeShader.Shader = shader;
        
            m_UniqueActiveShaders[shaderId] = activeShader;
        }

        m_UniqueActiveShaders[shaderId].UsageCount++;
    }

    void ShaderRenderSet::Remove(const std::shared_ptr<Shader>& shader)
    {
        assert(shader);

        const unsigned int shaderId = shader->GetRendererID();

        m_UniqueActiveShaders[shaderId].UsageCount--;

        if(m_UniqueActiveShaders[shaderId].UsageCount <= 0)
        {
            m_UniqueActiveShaders.erase(shaderId);
        }
    }

    bool ShaderRenderSet::Contains(const std::shared_ptr<Shader>& shader)
    {
        assert(shader);
        return m_UniqueActiveShaders.find(shader->GetRendererID()) != m_UniqueActiveShaders.end();
    }

    void ShaderRenderSet::Clear()
    {
        m_UniqueActiveShaders.clear();
    }
}
