#pragma once
#include <memory>

#include "LegacyCamera.h"
#include "LegacyFlyCameraController.h"
#include "Test.h"
#include "glm/glm.hpp"

class LegacyTexture;
class LegacyShader;
class LegacyIndexBuffer;
class LegacyVertexBuffer;
class LegacyVertexArray;

namespace tests
{
    class TestCamera : public Test
    {
    public:
        TestCamera();
        ~TestCamera() override;

        void Setup(GLFWwindow* Window) override;
        void Shutdown(GLFWwindow* Window) override;
        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<LegacyVertexArray> m_VAO;
        std::unique_ptr<LegacyVertexBuffer> m_VBO;
        std::unique_ptr<LegacyIndexBuffer> m_IBO;
        std::unique_ptr<LegacyShader> m_Shader;
        std::unique_ptr<LegacyTexture> m_Texture;

        static constexpr int m_TotalCubes = 5;
        int m_CameraObjectFocusId{0};
        glm::vec3 m_Positions[m_TotalCubes];
        glm::vec3 m_Scales[m_TotalCubes];
        glm::vec3 m_Rotations[m_TotalCubes];
        glm::mat4 m_MVP;

        std::shared_ptr<LegacyCamera> m_Camera;
        std::unique_ptr<LegacyFlyCameraController> m_CameraController;

        glm::mat4 GetRotationMatrix(const glm::vec3& EulerRotation) const;
        void UpdateCameraOrbitAround();
    };
}
