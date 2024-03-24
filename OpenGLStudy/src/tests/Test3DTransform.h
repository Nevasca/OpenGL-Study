#pragma once
#include <memory>

#include "Test.h"
#include "glm/glm.hpp"

class Texture;
class LegacyShader;
class IndexBuffer;
class VertexBuffer;
class VertexArray;

namespace tests
{
    class Test3DTransform : public Test
    {
    public:
        Test3DTransform();
        ~Test3DTransform() override;

        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<LegacyShader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        static constexpr int m_TotalCubes = 5;
        glm::vec3 m_Positions[m_TotalCubes];
        glm::vec3 m_Scales[m_TotalCubes];
        glm::vec3 m_Rotations[m_TotalCubes];
        glm::mat4 m_View;
        glm::mat4 m_Proj;
        glm::mat4 m_MVP;

        glm::mat4 GetRotationMatrix(const glm::vec3& EulerRotation) const;
    };
}
