#pragma once
#include <memory>

#include "IndexBuffer.h"
#include "LegacyShader.h"
#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
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

        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<LegacyShader> m_Shader;
        std::unique_ptr<Texture> m_Texture;
        
        glm::mat4 m_Proj;
        glm::mat4 m_View;
        glm::vec3 m_translationA{glm::vec3{200.f, 200.f, 0.f}};
        glm::vec3 m_translationB{glm::vec3{400.f, 200.f, 0.f}};
    };
}
