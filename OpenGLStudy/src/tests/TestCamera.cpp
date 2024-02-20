#include <GL/glew.h> // It throws that "gl.h included before glew.h" if we don't include it before the glfw3
#include <GLFW/glfw3.h>

#include "TestCamera.h"

#include "GameTime.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glm/detail/type_quat.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/ext/scalar_common.hpp"
#include "glm/gtc/quaternion.hpp"
#include "imgui/imgui.h"

namespace tests
{
    // Anonymous namespace is preferred over using static for vars and methods you don't want to accidentally expose to other translation units 
    namespace
    {
        // Using a global var instance of "this" so we can use the instance on required global function callback
        // for glfw scroll callback
        // This is a workaround, not sure what's the best approach on these cases...
        TestCamera* g_TestCamera;

        void ScrollCallbackWrapper(GLFWwindow* Window, double XOffset, double YOffset)
        {
            g_TestCamera->UpdateCameraZoom(Window, XOffset, YOffset);
        }
    }
    
    TestCamera::TestCamera()
    {
        m_VAO = std::make_unique<VertexArray>();

        float vertices[]
        {
            // Position                 UV
            // Front face
             -0.5f, -0.5f, -0.5f,   0.f, 0.f,
              0.5f, -0.5f, -0.5f,   1.f, 0.f,
              0.5f,  0.5f, -0.5f,   1.f, 1.f,
             -0.5f,  0.5f, -0.5f,   0.f, 1.f,

            // Back face
             -0.5f, -0.5f,  0.5f,   0.f, 0.f,
              0.5f, -0.5f,  0.5f,   1.f, 0.f,
              0.5f,  0.5f,  0.5f,   1.f, 1.f,
             -0.5f,  0.5f,  0.5f,   0.f, 1.f,

            // Right face
              0.5f, -0.5f, -0.5f,   0.f, 0.f,
              0.5f, -0.5f,  0.5f,   1.f, 0.f,
              0.5f,  0.5f,  0.5f,   1.f, 1.f,
              0.5f,  0.5f, -0.5f,   0.f, 1.f,

            // Left face
              -0.5f, -0.5f, -0.5f,   0.f, 0.f,
              -0.5f, -0.5f,  0.5f,   1.f, 0.f,
              -0.5f,  0.5f,  0.5f,   1.f, 1.f,
              -0.5f,  0.5f, -0.5f,   0.f, 1.f,

            // Top face
             -0.5f,  0.5f, -0.5f,   0.f, 0.f,
              0.5f,  0.5f, -0.5f,   1.f, 0.f,
              0.5f,  0.5f,  0.5f,   1.f, 1.f,
             -0.5f,  0.5f,  0.5f,   0.f, 1.f,

            // Bottom face
             -0.5f,  -0.5f, -0.5f,   0.f, 0.f,
              0.5f,  -0.5f, -0.5f,   1.f, 0.f,
              0.5f,  -0.5f,  0.5f,   1.f, 1.f,
             -0.5f,  -0.5f,  0.5f,   0.f, 1.f,
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

        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices), false);

        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VBO, layout);

        m_IBO = std::make_unique<IndexBuffer>(indices, 6 * 6);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Texture = std::make_unique<Texture>("res/textures/FancyPigeon.png");
        
        m_Shader->Bind();
        m_Texture->Bind(0);
        m_Shader->SetUniform1i("u_Texture", 0);

        m_Proj = glm::perspective(glm::radians(m_CameraFov), 960.f / 540.f, 0.1f, 100.f);

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
    }

    TestCamera::~TestCamera()
    { }
    
    void TestCamera::Setup(GLFWwindow* Window)
    {
        // We need to disable the cursor so we can "capture" it (invisible and always on center).
        // We will need that to proper move camera around when we move the mouse
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Set last known cursor position when entering to prevent large sudden jump on first focus
        double cursorX, cursorY;
        glfwGetCursorPos(Window, &cursorX, &cursorY);

        m_CursorLastX = cursorX;
        m_CursorLastY = cursorY;

        g_TestCamera = this;
        glfwSetScrollCallback(Window, ScrollCallbackWrapper);
    }
    
    void TestCamera::Shutdown(GLFWwindow* Window)
    {
        // Restore cursor default state
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        g_TestCamera = nullptr;
        glfwSetScrollCallback(Window, nullptr);
    }

    void TestCamera::OnProcessInput(GLFWwindow* Window)
    {
        UpdateCameraPosition(Window);
        UpdateCameraRotation(Window);
    }

    void TestCamera::UpdateCameraPosition(GLFWwindow* Window)
    {
        const float cameraSpeed = m_CameraSpeed * GameTime::DeltaTime;

        if(glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
        {
            m_CameraPos += cameraSpeed * m_CameraFront;
        }

        if(glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
        {
            m_CameraPos -= cameraSpeed * m_CameraFront;
        }

        if(glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
        {
            glm::vec3 cameraRight = glm::normalize(glm::cross(m_CameraFront, m_CameraUp));
            m_CameraPos -= cameraRight * cameraSpeed;
        }

        if(glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
        {
            glm::vec3 cameraRight = glm::normalize(glm::cross(m_CameraFront, m_CameraUp));
            m_CameraPos += cameraRight * cameraSpeed;
        }
    }

    void TestCamera::UpdateCameraRotation(GLFWwindow* Window)
    {
        double cursorX, cursorY;
        glfwGetCursorPos(Window, &cursorX, &cursorY);

        float xOffset = cursorX - m_CursorLastX;
        float yOffset = m_CursorLastY - cursorY; // Reversed since y-coord range from bottom to top
        m_CursorLastX = cursorX;
        m_CursorLastY = cursorY;

        const float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        m_CameraYaw += xOffset;
        m_CameraPitch += yOffset;

        // Clamp 
        m_CameraPitch = glm::clamp(m_CameraPitch, -89.f, 89.f);

        // Calculate Euler angles for the new camera forward based on pitch and yaw (we are not interested in roll for that)
        // For better math explanation, check https://learnopengl.com/Getting-started/Camera
        glm::vec3 cameraDirection{};
        cameraDirection.x = cos(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
        cameraDirection.y = sin(glm::radians(m_CameraPitch));
        cameraDirection.z = sin(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));

        m_CameraFront = glm::normalize(cameraDirection);
    }

    void TestCamera::UpdateCameraZoom(GLFWwindow* Window, double XScrollOffset, double YScrollOffset)
    {
        m_CameraFov -= static_cast<float>(YScrollOffset);
        m_CameraFov = glm::clamp(m_CameraFov, 1.f, 45.f);
        
        m_Proj = glm::perspective(glm::radians(m_CameraFov), 960.f / 540.f, 0.1f, 100.f);
    }

    void TestCamera::OnUpdate(float DeltaTime)
    {
        // UpdateCameraOrbitAround();
        UpdateCameraWalkAround();
    }

    void TestCamera::UpdateCameraOrbitAround()
    {
        m_CameraObjectFocusId = glm::clamp(m_CameraObjectFocusId, 0, m_TotalCubes - 1);

        const float radius = 10.f; // Radius to rotate the camera arround
        // float camX = sin(GameTime::Time) * radius;
        // float camZ = cos(GameTime::Time) * radius;
        float camX = sin(GameTime::Time) * radius + m_Positions[m_CameraObjectFocusId].x;
        float camZ = cos(GameTime::Time) * radius + m_Positions[m_CameraObjectFocusId].z;
        
        m_View = glm::mat4(1.f);
        // Cam position, position where the camera is looking at, up axis
        // m_View = glm::lookAt(glm::vec3(camX, 0.f, camZ), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        m_View = glm::lookAt(glm::vec3(camX, 0.f, camZ), m_Positions[m_CameraObjectFocusId], glm::vec3(0.f, 1.f, 0.f));
    }

    void TestCamera::UpdateCameraWalkAround()
    {
        m_View = glm::mat4(1.f);

        // Since the camera front is (0.f, 0.f, -1.f), adding it to the position will make the target position be in front of the camera
        // Ensuring that however we move, the camera keeps looking at the target direction (0.f, 0.f, -1.f)
        m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
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
        
            m_MVP = m_Proj * m_View * model;
            
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
    { }
}
