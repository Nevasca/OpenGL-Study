#pragma once

#include <memory>

#include "Camera.h"
#include "FlyCameraController.h"
#include "Test.h"
#include "glm/glm.hpp"

class Texture;
class Shader;
class IndexBuffer;
class VertexBuffer;
class VertexArray;

namespace tests
{
    class TestBasicLightingCasters : public Test
    {
    public:
        TestBasicLightingCasters();
        ~TestBasicLightingCasters() override;

        void Setup(GLFWwindow* Window) override;
        void Shutdown(GLFWwindow* Window) override;
        void OnProcessInput(GLFWwindow* Window) override;
        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<VertexArray> m_CubeVAO;
        std::unique_ptr<VertexBuffer> m_CubeVBO;
        std::unique_ptr<IndexBuffer> m_CubeIBO;
        std::unique_ptr<Shader> m_CubeShader;
        std::unique_ptr<Texture> m_CubeTextureDiff;
        std::unique_ptr<Texture> m_CubeTextureSpec;
        std::unique_ptr<Texture> m_CubeTextureEmission;

        std::unique_ptr<VertexArray> m_LightCubeVAO;
        std::unique_ptr<VertexBuffer> m_LightCubeVBO;
        std::unique_ptr<IndexBuffer> m_LightCubeIBO;
        std::unique_ptr<Shader> m_LightCubeShader;

        static constexpr int m_TotalCubes = 5;
        glm::vec3 m_Positions[m_TotalCubes];
        glm::vec3 m_Scales[m_TotalCubes];
        glm::vec3 m_Rotations[m_TotalCubes];

        bool m_OrbitLightEnabled{false};
        glm::vec3 m_LightSourcePosition{1.2f, 1.f, 2.f};
        glm::vec3 m_LightSourceScale{0.2f, 0.2f, 0.2f};
        glm::vec3 m_LightSourceRotation{130.f, 0.f, 0.f};
        glm::vec3 m_LightAmbientColor{0.2f, 0.2f, 0.2f};
        glm::vec3 m_LightDiffuseColor{0.5f, 0.5f, 0.5f};
        glm::vec3 m_LightSpecularColor{1.f, 1.f, 1.f};
        float m_LightConstant{1.f};
        float m_LightLinear{0.09f};
        float m_LightQuadratic{0.032f};
        float m_LightCutoff{12.5f};
        float m_LightOuterCutoff{17.5f};

        float m_CubeShininess{32.f};
        
        glm::mat4 m_MVP;

        std::shared_ptr<Camera> m_Camera;
        std::unique_ptr<FlyCameraController> m_CameraController;

        void CreateCube();
        void CreateLightSource();
        glm::mat4 GetRotationMatrix(const glm::vec3& EulerRotation) const;
        glm::vec3 GetForwardVector(const glm::vec3& EulerRotation) const;
    };
}
