#include <GL/glew.h> // It throws that "gl.h included before glew.h" if we don't include it before the glfw3
#include <GLFW/glfw3.h>

#include "TestBasicLighting.h"

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
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/ext/scalar_common.hpp"
#include "glm/gtc/quaternion.hpp"
#include "imgui/imgui.h"

namespace tests
{
    TestBasicLighting::TestBasicLighting()
    {
        CreateCube();
        CreateLightSource();

        m_Camera = std::make_shared<LegacyCamera>(960.f, 540.f);
        m_Camera->Position.z = 5.f;

        m_CameraController = std::make_unique<LegacyFlyCameraController>(m_Camera);
    }

    void TestBasicLighting::CreateCube()
    {
        m_CubeVAO = std::make_unique<LegacyVertexArray>();

        float vertices[]
        {
             // Position                 Normal           UV
             // Front face
             -0.5f, -0.5f,  0.5f,    0.f, 0.f, 1.f,    0.f, 0.f,
              0.5f, -0.5f,  0.5f,    0.f, 0.f, 1.f,    1.f, 0.f,
              0.5f,  0.5f,  0.5f,    0.f, 0.f, 1.f,    1.f, 1.f,
             -0.5f,  0.5f,  0.5f,    0.f, 0.f, 1.f,    0.f, 1.f,

             // Back face
             -0.5f, -0.5f, -0.5f,    0.f, 0.f, -1.f,   0.f, 0.f,
              0.5f, -0.5f, -0.5f,    0.f, 0.f, -1.f,   1.f, 0.f,
              0.5f,  0.5f, -0.5f,    0.f, 0.f, -1.f,   1.f, 1.f,
             -0.5f,  0.5f, -0.5f,    0.f, 0.f, -1.f,   0.f, 1.f, 

             // Right face
              0.5f, -0.5f,  0.5f,    1.f, 0.f, 0.f,    0.f, 0.f,
              0.5f, -0.5f, -0.5f,    1.f, 0.f, 0.f,    1.f, 0.f,
              0.5f,  0.5f, -0.5f,    1.f, 0.f, 0.f,    1.f, 1.f,
              0.5f,  0.5f,  0.5f,    1.f, 0.f, 0.f,    0.f, 1.f,

             // Left face
             -0.5f, -0.5f,  0.5f,   -1.f, 0.f, 0.f,    0.f, 0.f,
             -0.5f, -0.5f, -0.5f,   -1.f, 0.f, 0.f,    1.f, 0.f,
             -0.5f,  0.5f, -0.5f,   -1.f, 0.f, 0.f,    1.f, 1.f,
             -0.5f,  0.5f,  0.5f,   -1.f, 0.f, 0.f,    0.f, 1.f,

             // Top face
             -0.5f,  0.5f,  0.5f,    0.f, 1.f, 0.f,    0.f, 0.f,
              0.5f,  0.5f,  0.5f,    0.f, 1.f, 0.f,    1.f, 0.f,
              0.5f,  0.5f, -0.5f,    0.f, 1.f, 0.f,    1.f, 1.f,
             -0.5f,  0.5f, -0.5f,    0.f, 1.f, 0.f,    0.f, 1.f,

             // Bottom face
             -0.5f,  -0.5f,  0.5f,   0.f, -1.f, 0.f,   0.f, 0.f,
              0.5f,  -0.5f,  0.5f,   0.f, -1.f, 0.f,   1.f, 0.f,
              0.5f,  -0.5f, -0.5f,   0.f, -1.f, 0.f,   1.f, 1.f,
             -0.5f,  -0.5f, -0.5f,   0.f, -1.f, 0.f,   0.f, 1.f
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

        m_CubeVBO = std::make_unique<LegacyVertexBuffer>(vertices, sizeof(vertices), false);

        LegacyVertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_CubeVAO->AddBuffer(*m_CubeVBO, layout);

        m_CubeIBO = std::make_unique<LegacyIndexBuffer>(indices, 6 * 6);

        m_CubeShader = std::make_unique<LegacyShader>("res/shaders/BasicLitCustomMaps.shader");
        m_CubeShader->Bind();
        m_CubeShader->SetUniform1i("u_Material.diffuse", 0);
        m_CubeShader->SetUniform1i("u_Material.specular", 1);
        m_CubeShader->SetUniform1i("u_Material.emission", 2);

        m_CubeTextureDiff = std::make_unique<LegacyTexture>("res/textures/Container_Diff.png");
        m_CubeTextureSpec = std::make_unique<LegacyTexture>("res/textures/Container_Spec.png");
        m_CubeTextureEmission = std::make_unique<LegacyTexture>("res/textures/Container_Emission.png");
        m_CubeTextureDiff->Bind(0);
        m_CubeTextureSpec->Bind(1);
        m_CubeTextureEmission->Bind(2);

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
            // m_Rotations[i] = { 55 + i % 360, 55 + i % 360, 55 + i % 360};
            m_Rotations[i] = { 0.f, 0.f, 0.f};
        }
    }

    // Create a cube to act as light point,
    // it's just so we can understand where the light position is, the cube itself doesn't emit light 
    void TestBasicLighting::CreateLightSource()
    {
        m_LightCubeVAO = std::make_unique<LegacyVertexArray>();

        float vertices[]
        {
            // Position           
            // Front face
             -0.5f, -0.5f, 0.5f,  
              0.5f, -0.5f, 0.5f,  
              0.5f,  0.5f, 0.5f,  
             -0.5f,  0.5f, 0.5f,  

            // Back face
             -0.5f, -0.5f,  -0.5f,
              0.5f, -0.5f,  -0.5f,
              0.5f,  0.5f,  -0.5f,
             -0.5f,  0.5f,  -0.5f,

            // Right face
              0.5f, -0.5f, 0.5f,  
              0.5f, -0.5f, -0.5f, 
              0.5f,  0.5f, -0.5f, 
              0.5f,  0.5f, 0.5f,  

            // Left face
              -0.5f, -0.5f, 0.5f, 
              -0.5f, -0.5f, -0.5f,
              -0.5f,  0.5f, -0.5f,
              -0.5f,  0.5f, 0.5f, 

            // Top face
             -0.5f,  0.5f, 0.5f,  
              0.5f,  0.5f, 0.5f,  
              0.5f,  0.5f, -0.5f, 
             -0.5f,  0.5f, -0.5f, 

            // Bottom face
             -0.5f,  -0.5f, 0.5f, 
              0.5f,  -0.5f, 0.5f, 
              0.5f,  -0.5f, -0.5f,
             -0.5f,  -0.5f, -0.5f,
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

        m_LightCubeVBO = std::make_unique<LegacyVertexBuffer>(vertices, sizeof(vertices), false);

        LegacyVertexBufferLayout layout{};
        layout.Push<float>(3);
        m_LightCubeVAO->AddBuffer(*m_LightCubeVBO, layout);

        m_LightCubeIBO = std::make_unique<LegacyIndexBuffer>(indices, 6 * 6);

        m_LightCubeShader = std::make_unique<LegacyShader>("res/shaders/BasicLightCube.shader");
    }

    TestBasicLighting::~TestBasicLighting()
    { }
    
    void TestBasicLighting::Setup(GLFWwindow* Window)
    {
        m_CameraController->Setup(false);
    }
    
    void TestBasicLighting::Shutdown(GLFWwindow* Window)
    {
        m_CameraController->Shutdown();
    }

    void TestBasicLighting::OnUpdate(float DeltaTime)
    {
        m_CameraController->Update();

        const float orbitRadius = 4.5f;
        m_LightSourcePosition.x = sin(GameTime::Time) * orbitRadius;
        m_LightSourcePosition.z = cos(GameTime::Time) * orbitRadius;
    }

    void TestBasicLighting::OnRender()
    {
        Renderer renderer{};

        // Draw light source visualization cube
        {
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, m_LightSourcePosition);
            model = glm::scale(model, m_LightSourceScale);
            m_MVP = m_Camera->GetViewProjectionMatrix() * model;

            m_LightCubeShader->Bind();
            m_LightCubeShader->SetUniformMat4f("u_MVP", m_MVP);
            m_LightCubeShader->SetUniform3f("u_Color", m_LightDiffuseColor);

            renderer.Draw(*m_LightCubeVAO, *m_LightCubeIBO, *m_LightCubeShader);
        }

        // Draw cubes
        {
            m_CubeShader->Bind();
            m_CubeShader->SetUniform3f("u_Light.position", m_LightSourcePosition);
            m_CubeShader->SetUniform3f("u_Light.ambient", m_LightAmbientColor);
            m_CubeShader->SetUniform3f("u_Light.diffuse", m_LightDiffuseColor);
            m_CubeShader->SetUniform3f("u_Light.specular", m_LightSpecularColor);

            // m_CubeShader->SetUniform3f("u_Material.ambient", m_CubeAmbientColor);
            // m_CubeShader->SetUniform3f("u_Material.diffuse", m_CubeDiffuseColor); // We are now using a diffuse map on setup (texture)
            // m_CubeShader->SetUniform3f("u_Material.specular", m_CubeSpecularColor);
            m_CubeShader->SetUniform1f("u_Material.shininess", m_CubeShininess);

            // If we were calculating light in view space, we wouldn't need to pass the view position
            // since it would be 0,0,0
            m_CubeShader->SetUniform3f("u_ViewPosition", m_Camera->Position);
            
            for(int i = 0; i < m_TotalCubes; i++)
            {
                glm::mat4 model = glm::mat4(1.f);
        
                model = glm::translate(model, m_Positions[i]);
                model *= GetRotationMatrix(m_Rotations[i]);
                model = glm::scale(model, m_Scales[i]);
            
                m_MVP = m_Camera->GetViewProjectionMatrix() * model;
        
                m_CubeShader->SetUniformMat4f("u_MVP", m_MVP);
                
                // We will need the model matrix as well, so we can do light calculations in world space
                m_CubeShader->SetUniformMat4f("u_Model", model);  
                
                renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_CubeShader);
            }
        }
    }

    glm::mat4 TestBasicLighting::GetRotationMatrix(const glm::vec3& EulerRotation) const
    {
        glm::quat rotationQuat = glm::quat(glm::radians(EulerRotation));
        glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
        
        return rotationMatrix;
    }

    void TestBasicLighting::OnImGuiRender()
    {
        if(ImGui::CollapsingHeader("Light"))
        {
            ImGui::ColorEdit3("L Ambient", &m_LightAmbientColor.x);
            ImGui::ColorEdit3("L Diffuse", &m_LightDiffuseColor.x);
            ImGui::ColorEdit3("L Specular", &m_LightSpecularColor.x);
        }

        if(ImGui::CollapsingHeader("Cube"))
        {
            // ImGui::ColorEdit3("C Ambient Color", &m_CubeAmbientColor.x);
            // ImGui::ColorEdit3("C Diffuse Color", &m_CubeDiffuseColor.x);
            // ImGui::ColorEdit3("C Specular Color", &m_CubeSpecularColor.x);
            ImGui::InputFloat("C Shininess", &m_CubeShininess);
        }
        
        m_CameraController->OnImGuiRender();
    }
}
