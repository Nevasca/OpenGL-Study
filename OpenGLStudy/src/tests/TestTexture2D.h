#pragma once
#include <memory>

#include "legacy/LegacyIndexBuffer.h"
#include "legacy/LegacyShader.h"
#include "Test.h"
#include "legacy/LegacyTexture.h"
#include "legacy/LegacyVertexArray.h"
#include "legacy/LegacyVertexBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace tests
{
    class TestTexture2D : public Test
    {
    public:

        TestTexture2D();
        ~TestTexture2D() override;

        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<LegacyVertexArray> m_VertexArray;
        std::unique_ptr<LegacyVertexBuffer> m_VertexBuffer;
        std::unique_ptr<LegacyIndexBuffer> m_IndexBuffer;
        std::unique_ptr<LegacyShader> m_Shader;
        std::unique_ptr<LegacyTexture> m_Texture;
        
        glm::mat4 m_Proj;
        glm::mat4 m_View;
        glm::vec3 m_translationA{glm::vec3{200.f, 200.f, 0.f}};
        glm::vec3 m_translationB{glm::vec3{400.f, 200.f, 0.f}};
    };
}
