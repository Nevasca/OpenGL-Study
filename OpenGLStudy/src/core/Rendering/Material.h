#pragma once
#include <map>
#include <memory>
#include <string>

#include "glm/fwd.hpp"
#include "glm/vec4.hpp"

class Texture;
class Shader;

struct MaterialTextureProperty
{
    std::shared_ptr<Texture> Texture{};
    unsigned int Slot{0};
};

class Material
{
public:
    
    void SetColor(const std::string& name, const glm::vec4& color);
    void SetTexture(const std::string& name, const std::shared_ptr<Texture>& texture, unsigned int slot);
    void SetMat4(const std::string& name, const glm::mat4& matrix) const;
    void Bind() const;
    void Unbind() const;

    void SetShader(const std::shared_ptr<Shader>& shader) { m_Shader = shader; }
    const std::shared_ptr<Shader>& GetShader() const { return m_Shader; }
    void SetId(unsigned int id) { m_Id = id; }
    unsigned int GetId() const { return m_Id; }

private:

    std::shared_ptr<Shader> m_Shader{};
    std::map<std::string, glm::vec4> m_ColorProperties{};
    std::map<std::string, MaterialTextureProperty> m_TextureProperties{};
    unsigned int m_Id{0};
};