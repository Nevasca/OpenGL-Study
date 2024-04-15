#include <GL/glew.h> // It throws that "gl.h included before glew.h" if we don't include it before the glfw3
#include <GLFW/glfw3.h>

#include "TestCamera.h"

#include <memory>

#include "GameTime.h"
#include "legacy/LegacyIndexBuffer.h"
#include "legacy/Renderer.h"
#include "legacy/LegacyShader.h"
#include "legacy/LegacyTexture.h"
#include "legacy/LegacyVertexArray.h"
#include "legacy/LegacyVertexBuffer.h"
#include "legacy/LegacyVertexBufferLayout.h"
#include "glm/detail/type_quat.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/ext/scalar_common.hpp"
#include "glm/gtc/quaternion.hpp"
#include "imgui/imgui.h"

namespace tests
{
    TestCamera::TestCamera()
    {
        m_VAO = std::make_unique<LegacyVertexArray>();

        float vertices[]
        {
            // Position                 UV
            // Front face
             -0.5f, -0.5f, 0.5f,    0.f, 0.f,
              0.5f, -0.5f, 0.5f,    1.f, 0.f,
              0.5f,  0.5f, 0.5f,    1.f, 1.f,
             -0.5f,  0.5f, 0.5f,    0.f, 1.f,

            // Back face
             -0.5f, -0.5f,  -0.5f,  0.f, 0.f,
              0.5f, -0.5f,  -0.5f,  1.f, 0.f,
              0.5f,  0.5f,  -0.5f,  1.f, 1.f,
             -0.5f,  0.5f,  -0.5f,  0.f, 1.f,

            // Right face
              0.5f, -0.5f, 0.5f,    0.f, 0.f,
              0.5f, -0.5f, -0.5f,   1.f, 0.f,
              0.5f,  0.5f, -0.5f,   1.f, 1.f,
              0.5f,  0.5f, 0.5f,    0.f, 1.f,

            // Left face
              -0.5f, -0.5f, 0.5f,    0.f, 0.f,
              -0.5f, -0.5f, -0.5f,   1.f, 0.f,
              -0.5f,  0.5f, -0.5f,   1.f, 1.f,
              -0.5f,  0.5f, 0.5f,    0.f, 1.f,

            // Top face
             -0.5f,  0.5f, 0.5f,    0.f, 0.f,
              0.5f,  0.5f, 0.5f,    1.f, 0.f,
              0.5f,  0.5f, -0.5f,   1.f, 1.f,
             -0.5f,  0.5f, -0.5f,   0.f, 1.f,

            // Bottom face
             -0.5f,  -0.5f, 0.5f,    0.f, 0.f,
              0.5f,  -0.5f, 0.5f,    1.f, 0.f,
              0.5f,  -0.5f, -0.5f,   1.f, 1.f,
             -0.5f,  -0.5f, -0.5f,   0.f, 1.f,
        };

        unsigned int indices[]
        {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        m_VBO = std::make_unique<LegacyVertexBuffer>(vertices, sizeof(vertices), false);

        LegacyVertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VBO, layout);

        m_IBO = std::make_unique<LegacyIndexBuffer>(indices, 6 * 6);

        m_Shader = std::make_unique<LegacyShader>("res/shaders/Basic.shader");
        m_Texture = std::make_unique<LegacyTexture>("res/textures/FancyPigeon.png");
        
        m_Shader->Bind();
        m_Texture->Bind(0);
        m_Shader->SetUniform1i("u_Texture", 0);

        glm::vec3 initialPositions []
        {
            {0.f, 0.f, 0.f},
            {1.5f, 1.5f, -2.5f},
            {-1.5, 1.5f, -2.5f},
            {1.5f, -1.5f, -2.5f},
            {-1.5, -1.5f, -2.5f},
        };
        
        for(int i = 0; i < m_TotalCubes; i++)
        {
            
            m_Positions[i] = initialPositions[i];
            m_Scales[i] = {1.f, 1.f, 1.f};
            m_Rotations[i] = { 55 + i % 360, 55 + i % 360, 55 + i % 360};
        }

        m_Camera = std::make_shared<LegacyCamera>(960.f, 540.f);
        m_CameraController = std::make_unique<LegacyFlyCameraController>(m_Camera);
    }

    TestCamera::~TestCamera()
    { }
    
    void TestCamera::Setup(GLFWwindow* Window)
    {
        m_CameraController->Setup();
    }
    
    void TestCamera::Shutdown(GLFWwindow* Window)
    {
        m_CameraController->Shutdown();
    }

    void TestCamera::OnUpdate(float DeltaTime)
    {
        m_CameraController->Update();
        // UpdateCameraOrbitAround();
    }

    void TestCamera::UpdateCameraOrbitAround()
    {
        // m_CameraObjectFocusId = glm::clamp(m_CameraObjectFocusId, 0, m_TotalCubes - 1);
        //
        // const float radius = 10.f; // Radius to rotate the camera arround
        // // float camX = sin(GameTime::Time) * radius;
        // // float camZ = cos(GameTime::Time) * radius;
        // float camX = sin(GameTime::Time) * radius + m_Positions[m_CameraObjectFocusId].x;
        // float camZ = cos(GameTime::Time) * radius + m_Positions[m_CameraObjectFocusId].z;
        //
        // m_View = glm::mat4(1.f);
        // // Cam position, position where the camera is looking at, up axis
        // // m_View = glm::lookAt(glm::vec3(camX, 0.f, camZ), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        // m_View = glm::lookAt(glm::vec3(camX, 0.f, camZ), m_Positions[m_CameraObjectFocusId], glm::vec3(0.f, 1.f, 0.f));
    }

    void TestCamera::OnRender()
    {
        Renderer renderer{};

        m_Shader->Bind();

        for(int i = 0; i < m_TotalCubes; i++)
        {
            glm::mat4 model = glm::mat4(1.f);

            model = glm::translate(model, m_Positions[i]);
            model *= GetRotationMatrix(m_Rotations[i]);
            model = glm::scale(model, m_Scales[i]);
        
            m_MVP = m_Camera->GetViewProjectionMatrix() * model;
            
            m_Shader->SetUniformMat4f("u_MVP", m_MVP);
            
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
    }

    glm::mat4 TestCamera::GetRotationMatrix(const glm::vec3& EulerRotation) const
    {
        glm::quat rotationQuat = glm::quat(glm::radians(EulerRotation));
        glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
        
        return rotationMatrix;
    }

    void TestCamera::OnImGuiRender()
    {
        m_CameraController->OnImGuiRender();
        
        std::string rotationText = "Camera Rotation: ";
        glm::vec3 rotation = m_Camera->GetRotation();
        rotationText += std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + ", " + std::to_string(rotation.z);
        ImGui::Text(rotationText.c_str());

        std::string positionText = "Camera Position: ";
        glm::vec3 position = m_Camera->Position;
        positionText += std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z);
        ImGui::Text(positionText.c_str());
    }
}
