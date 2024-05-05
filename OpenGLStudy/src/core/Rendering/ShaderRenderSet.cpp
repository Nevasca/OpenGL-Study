#include "ShaderRenderSet.h"
#include "Shader.h"

namespace Rendering
{
    void ShaderRenderSet::Add(const std::shared_ptr<Shader>& shader)
    {
        const unsigned int shaderId = shader->GetRendererID();
        
        if(m_UniqueActiveShaders.find(shaderId) == m_UniqueActiveShaders.end())
        {
            ActiveShader activeShader{};
            activeShader.Shader = shader;
        
            m_UniqueActiveShaders[shaderId] = activeShader;
        }

        m_UniqueActiveShaders[shaderId].UsageCount++;
    }

    void ShaderRenderSet::Remove(const std::shared_ptr<Shader>& shader)
    {
        const unsigned int shaderId = shader->GetRendererID();

        m_UniqueActiveShaders[shaderId].UsageCount--;

        if(m_UniqueActiveShaders[shaderId].UsageCount <= 0)
        {
            m_UniqueActiveShaders.erase(shaderId);
        }
    }

    void ShaderRenderSet::Clear()
    {
        m_UniqueActiveShaders.clear();
    }
}
