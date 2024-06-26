#pragma once

#include <map>
#include <memory>
#include <string>

#include "Test.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class LegacyShader;
class LegacyIndexBuffer;
class LegacyVertexBuffer;
class LegacyVertexArray;

namespace tests
{
    struct Character
    {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2 Size; // Size of glyph
        glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
        unsigned int Advance; // Offset to advance to next glyph
    };
    
    class TestTextRendering : public Test
    {
    public:
        TestTextRendering();
        ~TestTextRendering() override;

        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::map<char, Character> m_Characters;
        std::unique_ptr<LegacyVertexArray> m_VAO;
        std::unique_ptr<LegacyVertexBuffer> m_VBO;
        std::unique_ptr<LegacyIndexBuffer> m_IBO;
        std::unique_ptr<LegacyShader> m_Shader;
        char m_Text[64] = "Hello World!";
        glm::vec2 m_Position{25.f, 25.f};
        float m_Scale{1.f};
        glm::vec3 m_Color{0.8f, 0.3f, 0.5f};

        void RenderText(const LegacyShader& Shader, const std::string& Text, float X, float Y, float Scale, const glm::vec3& Color);
    };
}
