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
    class Test2DTransform : public Test
    {
    public:
        Test2DTransform();
        ~Test2DTransform() override;

        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<LegacyVertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<LegacyIndexBuffer> m_IBO;
        std::unique_ptr<LegacyShader> m_Shader;
        std::unique_ptr<LegacyTexture> m_Texture;

        glm::vec3 m_Position{300.f, 300.f, 0.f};
        glm::vec3 m_Scale{3.f, 3.f, 3.f};
        glm::vec3 m_Rotation{0.f, 0.f, 0.f};
        float m_RotationZSpeed{0.f};
        glm::mat4 m_Proj;
        glm::mat4 m_MVP;

        glm::mat4 GetRotationMatrix() const;
    };
}
