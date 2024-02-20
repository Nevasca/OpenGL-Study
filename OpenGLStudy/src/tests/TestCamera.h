#pragma once
#include <memory>

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
        void UpdateCameraZoom(GLFWwindow* Window, double XScrollOffset, double YScrollOffset);

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
        glm::mat4 m_View;
        glm::mat4 m_Proj;
        glm::mat4 m_MVP;

        glm::vec3 m_CameraPos{0.f, 0.f, 3.f};
        glm::vec3 m_CameraFront{0.f, 0.f, -1.f};
        glm::vec3 m_CameraUp{0.f, 1.f, 0.f};
        float m_CameraSpeed{2.5f};
        float m_CursorLastX{960.f / 2}; // Initialize last cursor pos on center screen
        float m_CursorLastY{540.f / 2};
        float m_CameraYaw{-90.f}; // Makes the camera points towards negative z-axis by default
        float m_CameraPitch{0.f};
        float m_CameraFov{45.f};

        glm::mat4 GetRotationMatrix(const glm::vec3& EulerRotation) const;
        void UpdateCameraOrbitAround();
        void UpdateCameraWalkAround();
        void UpdateCameraPosition(GLFWwindow* Window);
        void UpdateCameraRotation(GLFWwindow* Window);
    };
}
