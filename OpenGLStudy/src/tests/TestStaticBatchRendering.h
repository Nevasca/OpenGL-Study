#pragma once

#include <memory>

#include "Test.h"
#include "glm/glm.hpp"

class Texture;
class LegacyShader;
class LegacyIndexBuffer;
class VertexBuffer;
class LegacyVertexArray;

namespace tests
{
    class TestStaticBatchRendering : public Test
    {
    public:

        TestStaticBatchRendering();
        ~TestStaticBatchRendering() override;

        void OnRender() override;

    private:

        std::unique_ptr<LegacyVertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<LegacyIndexBuffer> m_IndexBuffer;
        std::unique_ptr<LegacyShader> m_Shader;
        std::unique_ptr<Texture> m_TextureA;
        std::unique_ptr<Texture> m_TextureB;
        glm::mat4 m_Proj;
        glm::mat4 m_View;

    };
}
