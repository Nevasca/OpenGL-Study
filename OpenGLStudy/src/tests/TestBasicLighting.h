#pragma once

#include <memory>

#include "LegacyCamera.h"
#include "LegacyFlyCameraController.h"
#include "Test.h"
#include "glm/glm.hpp"

class Texture;
class LegacyShader;
class IndexBuffer;
class VertexBuffer;
class VertexArray;

namespace tests
{
    class TestBasicLighting : public Test
    {
    public:
        TestBasicLighting();
        ~TestBasicLighting() override;

        void Setup(GLFWwindow* Window) override;
        void Shutdown(GLFWwindow* Window) override;
        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<VertexArray> m_CubeVAO;
        std::unique_ptr<VertexBuffer> m_CubeVBO;
        std::unique_ptr<IndexBuffer> m_CubeIBO;
        std::unique_ptr<LegacyShader> m_CubeShader;
        std::unique_ptr<Texture> m_CubeTextureDiff;
        std::unique_ptr<Texture> m_CubeTextureSpec;
        std::unique_ptr<Texture> m_CubeTextureEmission;

        std::unique_ptr<VertexArray> m_LightCubeVAO;
        std::unique_ptr<VertexBuffer> m_LightCubeVBO;
        std::unique_ptr<IndexBuffer> m_LightCubeIBO;
        std::unique_ptr<LegacyShader> m_LightCubeShader;

        static constexpr int m_TotalCubes = 4;
        glm::vec3 m_Positions[m_TotalCubes];
        glm::vec3 m_Scales[m_TotalCubes];
        glm::vec3 m_Rotations[m_TotalCubes];
        
        glm::vec3 m_LightSourcePosition{1.2f, 1.f, 2.f};
        glm::vec3 m_LightSourceScale{0.2f, 0.2f, 0.2f};
        glm::vec3 m_LightAmbientColor{0.2f, 0.2f, 0.2f};
        glm::vec3 m_LightDiffuseColor{0.5f, 0.5f, 0.5f};
        glm::vec3 m_LightSpecularColor{1.f, 1.f, 1.f};

        // glm::vec3 m_CubeAmbientColor{1.f, 0.5f, 0.31f};
        // glm::vec3 m_CubeDiffuseColor{1.f, 0.5f, 0.31f};
        // glm::vec3 m_CubeSpecularColor{0.5f, 0.5f, 0.5f};
        float m_CubeShininess{32.f};
        
        glm::mat4 m_MVP;

        std::shared_ptr<LegacyCamera> m_Camera;
        std::unique_ptr<LegacyFlyCameraController> m_CameraController;

        void CreateCube();
        void CreateLightSource();
        glm::mat4 GetRotationMatrix(const glm::vec3& EulerRotation) const;
    };
}
