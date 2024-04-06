#pragma once

#include <memory>

#include "Test.h"
#include "glm/glm.hpp"

class LegacyTexture;
class LegacyShader;
class LegacyIndexBuffer;
class VertexBuffer;
class LegacyVertexArray;

namespace tests
{
    class TestDynamicBatchRendering : public Test
    {
    public:

        TestDynamicBatchRendering();
        ~TestDynamicBatchRendering() override;

        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<LegacyVertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<LegacyIndexBuffer> m_IndexBuffer;
        std::unique_ptr<LegacyShader> m_Shader;
        std::unique_ptr<LegacyTexture> m_TextureA;
        std::unique_ptr<LegacyTexture> m_TextureB;
        glm::mat4 m_Proj;
        glm::mat4 m_View;
        glm::vec2 m_translationA{100.f, 300.f};
        glm::vec2 m_translationB{300.f, 300.f};
    };
}
