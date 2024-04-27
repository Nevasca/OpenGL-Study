#include "Material.h"

#include "Shader.h"
#include "Texture.h"

void Material::SetColor(const std::string& name, const glm::vec4& color)
{
    m_ColorProperties[name] = color;
}

void Material::SetTexture(const std::string& name, const std::shared_ptr<Texture>& texture, unsigned int slot)
{
    if(m_TextureProperties.find(name) == m_TextureProperties.end())
    {
        m_TextureProperties[name] = MaterialTextureProperty{};
    }

    m_TextureProperties[name].Texture = texture;
    m_TextureProperties[name].Slot = slot;

    m_Shader->Bind();
    m_Shader->SetUniform1i(name, static_cast<int>(slot));
}

void Material::SetMat4(const std::string& name, const glm::mat4& matrix) const
{
    m_Shader->SetUniformMat4f(name, matrix);
}

void Material::SetBool(const std::string& name, const bool value)
{
    m_BoolProperties[name] = value;
}

void Material::Bind() const
{
    Bind(*m_Shader);
}

void Material::Bind(Shader& shader) const
{
    shader.Bind();

    for(const auto& propertyPair : m_ColorProperties)
    {
        shader.SetUniform4f(propertyPair.first, propertyPair.second);
    }

    for(const auto& propertyPair : m_TextureProperties)
    {
        const MaterialTextureProperty& textureProperty = propertyPair.second;
        textureProperty.Texture->Bind(textureProperty.Slot);
    }

    for(const auto& propertyPair : m_BoolProperties)
    {
        shader.SetUniform1i(propertyPair.first, propertyPair.second);
    }
}

void Material::Unbind() const
{
    Unbind(*m_Shader);
}

void Material::Unbind(const Shader& shader) const
{
    for(const auto& propertyPair : m_TextureProperties)
    {
        const MaterialTextureProperty& textureProperty = propertyPair.second;
        textureProperty.Texture->Unbind(textureProperty.Slot);
    }

    shader.Unbind();
}
