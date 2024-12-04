#pragma once
#include <map>
#include <memory>
#include <string>

#include "glm/fwd.hpp"
#include "glm/vec4.hpp"

namespace Rendering
{
    class Cubemap;
}

class Texture;
class Shader;

struct MaterialTextureProperty
{
    std::shared_ptr<Texture> Texture{};
    unsigned int Slot{0};
};

struct MaterialCubemapProperty
{
    std::shared_ptr<Rendering::Cubemap> Cubemap{};
    unsigned int Slot{0};
};

enum class MaterialRenderingMode : uint8_t
{
    Opaque,
    AlphaCutout,
    Transparent
};

class Material
{
public:
    
    void SetColor(const std::string& name, const glm::vec4& color);
    void SetTexture(const std::string& name, const std::shared_ptr<Texture>& texture, unsigned int slot);
    void SetCubemap(const std::string& name, const std::shared_ptr<Rendering::Cubemap>& cubemap, unsigned int slot);
    void SetMat4(const std::string& name, const glm::mat4& matrix) const;
    void SetBool(const std::string& name, const bool value);
    void SetFloat(const std::string& name, const float value);
    void SetInt(const std::string& name, const int value);
    void SetRenderingMode(MaterialRenderingMode renderingMode);
    void Bind() const;
    void Bind(Shader& shader) const;
    void Unbind() const;
    void Unbind(const Shader& shader) const;

    void SetShader(const std::shared_ptr<Shader>& shader) { m_Shader = shader; }
    const std::shared_ptr<Shader>& GetShader() const { return m_Shader; }
    void SetId(unsigned int id) { m_Id = id; }
    unsigned int GetId() const { return m_Id; }
    void SetName(const std::string& name) { m_Name = name; }
    std::string GetName() const { return m_Name; }
    MaterialRenderingMode GetRenderingMode() const { return m_RenderingMode; }
    const std::map<std::string, glm::vec4>& GetAllColorProperties() const { return m_ColorProperties; }
    const std::map<std::string, MaterialTextureProperty>& GetAllTextureProperties() const { return m_TextureProperties; }
    const std::map<std::string, bool>& GetAllBoolProperties() const { return m_BoolProperties; }
    const std::map<std::string, float>& GetAllFloatProperties() const { return m_FloatProperties; }
    const std::map<std::string, int>& GetAllIntProperties() const { return m_IntProperties; }

private:

    // TODO: have this set somewhere else and with defines/constexpr to not manually add it
    // 1 SKYBOX + 3 DIRECTIONAL SHADOW MAP + 20 POINT SHADOW MAP + 20 SPOT SHADOW MAP
    constexpr static int TOTAL_SYSTEM_RESERVED_TEXTURE_SLOTS = 44;
    
    unsigned int m_Id{0};
    std::shared_ptr<Shader> m_Shader{};
    std::map<std::string, glm::vec4> m_ColorProperties{};
    std::map<std::string, MaterialTextureProperty> m_TextureProperties{};
    std::map<std::string, bool> m_BoolProperties{};
    std::map<std::string, MaterialCubemapProperty> m_CubemapProperties{};
    std::map<std::string, float> m_FloatProperties{};
    std::map<std::string, int> m_IntProperties{};
    
    MaterialRenderingMode m_RenderingMode{MaterialRenderingMode::Opaque};
    std::string m_Name{};
};
