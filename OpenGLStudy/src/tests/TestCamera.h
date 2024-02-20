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
    class TestCamera : public Test
    {
    public:
        TestCamera();
        ~TestCamera() override;

        void Setup(GLFWwindow* Window) override;
        void Shutdown(GLFWwindow* Window) override;
        void OnProcessInput(GLFWwindow* Window) override;
        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        static constexpr int m_TotalCubes = 5;
        int m_CameraObjectFocusId{0};
        glm::vec3 m_Positions[m_TotalCubes];
        glm::vec3 m_Scales[m_TotalCubes];
        glm::vec3 m_Rotations[m_TotalCubes];
        glm::mat4 m_MVP;

        std::shared_ptr<Camera> m_Camera;
        std::unique_ptr<FlyCameraController> m_CameraController;

        glm::mat4 GetRotationMatrix(const glm::vec3& EulerRotation) const;
        void UpdateCameraOrbitAround();
    };
}
