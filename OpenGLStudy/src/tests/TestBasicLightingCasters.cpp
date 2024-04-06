#include <GL/glew.h> // It throws that "gl.h included before glew.h" if we don't include it before the glfw3
#include <GLFW/glfw3.h>

#include "TestBasicLightingCasters.h"

#include <memory>

#include "GameTime.h"
#include "LegacyIndexBuffer.h"
#include "Renderer.h"
#include "LegacyShader.h"
#include "LegacyTexture.h"
#include "LegacyVertexArray.h"
#include "VertexBuffer.h"
#include "LegacyVertexBufferLayout.h"
#include "glm/detail/type_quat.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/ext/scalar_common.hpp"
#include "glm/gtc/quaternion.hpp"
#include "imgui/imgui.h"

namespace tests
{
    TestBasicLightingCasters::TestBasicLightingCasters()
    {
        CreateCubes();
        CreateLightSources();

        m_Camera = std::make_shared<LegacyCamera>(960.f, 540.f);
        m_Camera->Position.z = 5.f;

        m_CameraController = std::make_unique<LegacyFlyCameraController>(m_Camera);
    }

    void TestBasicLightingCasters::CreateCubes()
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

        m_CubeVBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices), false);

        LegacyVertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_CubeVAO->AddBuffer(*m_CubeVBO, layout);

        m_CubeIBO = std::make_unique<LegacyIndexBuffer>(indices, 6 * 6);

        m_CubeShader = std::make_unique<LegacyShader>("res/shaders/BasicLit.shader");
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
            {0.f, 0.f, -3.5f},
        };
        
        for(int i = 0; i < TOTAL_CUBES; i++)
        {
            
            m_Positions[i] = initialPositions[i];
            m_Scales[i] = {1.f, 1.f, 1.f};
            // m_Rotations[i] = { 55 + i % 360, 55 + i % 360, 55 + i % 360};
            m_Rotations[i] = { 0.f, 0.f, 0.f};
        }
    }

    // Create a cube to act as light point,
    // it's just so we can understand where the light position is, the cube itself doesn't emit light 
    void TestBasicLightingCasters::CreateLightSources()
    {
        m_LightCubeVAO = std::make_unique<LegacyVertexArray>();

        // float vertices[]
        // {
        //     // Position           
        //     // Front face
        //      -0.5f, -0.5f, 0.5f,  
        //       0.5f, -0.5f, 0.5f,  
        //       0.5f,  0.5f, 0.5f,  
        //      -0.5f,  0.5f, 0.5f,  
        //
        //     // Back face
        //      -0.5f, -0.5f,  -0.5f,
        //       0.5f, -0.5f,  -0.5f,
        //       0.5f,  0.5f,  -0.5f,
        //      -0.5f,  0.5f,  -0.5f,
        //
        //     // Right face
        //       0.5f, -0.5f, 0.5f,  
        //       0.5f, -0.5f, -0.5f, 
        //       0.5f,  0.5f, -0.5f, 
        //       0.5f,  0.5f, 0.5f,  
        //
        //     // Left face
        //       -0.5f, -0.5f, 0.5f, 
        //       -0.5f, -0.5f, -0.5f,
        //       -0.5f,  0.5f, -0.5f,
        //       -0.5f,  0.5f, 0.5f, 
        //
        //     // Top face
        //      -0.5f,  0.5f, 0.5f,  
        //       0.5f,  0.5f, 0.5f,  
        //       0.5f,  0.5f, -0.5f, 
        //      -0.5f,  0.5f, -0.5f, 
        //
        //     // Bottom face
        //      -0.5f,  -0.5f, 0.5f, 
        //       0.5f,  -0.5f, 0.5f, 
        //       0.5f,  -0.5f, -0.5f,
        //      -0.5f,  -0.5f, -0.5f,
        // };

        float vertices[]
        {
            // Position           
           // Back face
            -0.5f,  -0.5f, -0.5f, 
             0.5f,  -0.5f, -0.5f, 
             0.5f,   0.5f, -0.5f,
            -0.5f,   0.5f, -0.5f,
            
           // Front Tip pyramid
             0.0f,  0.0f, 0.5f,  
       };

        unsigned int indices[]
        {
            0, 1, 2,
            2, 3, 0,

            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            3, 0, 4
        };

        m_LightCubeVBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices), false);

        LegacyVertexBufferLayout layout{};
        layout.Push<float>(3);
        m_LightCubeVAO->AddBuffer(*m_LightCubeVBO, layout);

        m_LightCubeIBO = std::make_unique<LegacyIndexBuffer>(indices, 6 + 4 * 3);

        m_LightCubeShader = std::make_unique<LegacyShader>("res/shaders/BasicLightCube.shader");

        for(int i = 0; i < TOTAL_LIGHT_POINTS; i++)
        {
            glm::vec3 position = {i, i + 1.f, i};
            m_PointLightPositions[i] = position;
            m_PointLightRotations[i] = {0.f, 0.f, 0.f};
            m_PointLightAmbientColors[i] = {0.2f, 0.2f, 0.2f};
            m_PointLightDiffuseColors[i] = {0.5f, 0.5f, 0.5f};
            m_PointLightSpecularColors[i] = {1.f, 1.f, 1.f};
            m_PointLightConstants[i] = 1.f;
            m_PointLightLinears[i] = 0.09f;
            m_PointLightQuadratics[i] = 0.032f;
        }
    }

    TestBasicLightingCasters::~TestBasicLightingCasters()
    { }
    
    void TestBasicLightingCasters::Setup(GLFWwindow* Window)
    {
        m_CameraController->Setup(false);
    }
    
    void TestBasicLightingCasters::Shutdown(GLFWwindow* Window)
    {
        m_CameraController->Shutdown();
    }

    void TestBasicLightingCasters::OnUpdate(float DeltaTime)
    {
        m_CameraController->Update();

        // if(!m_OrbitLightEnabled)
        // {
        //     return;
        // }
        //
        // const float orbitRadius = 4.5f;
        // m_PointLightPositions.x = sin(GameTime::Time) * orbitRadius;
        // m_PointLightPositions.z = cos(GameTime::Time) * orbitRadius;
    }

    void TestBasicLightingCasters::OnRender()
    {
        Renderer renderer{};

        // Draw light sources visualization cube
        {
            m_LightCubeShader->Bind();

            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, m_DirectionalLightPosition);
            model *= GetRotationMatrix(m_DirectionalLightRotation);
            model = glm::scale(model, m_LightCubeScale);
            m_MVP = m_Camera->GetViewProjectionMatrix() * model;

            m_LightCubeShader->SetUniformMat4f("u_MVP", m_MVP);
            m_LightCubeShader->SetUniform3f("u_Color", m_DirectionalLightDiffuseColor);

            renderer.Draw(*m_LightCubeVAO, *m_LightCubeIBO, *m_LightCubeShader);
            
            for(int i = 0; i < TOTAL_LIGHT_POINTS; i++)
            {
                model = glm::mat4(1.f);
                model = glm::translate(model, m_PointLightPositions[i]);
                model *= GetRotationMatrix(m_PointLightRotations[i]);
                model = glm::scale(model, m_LightCubeScale);
                m_MVP = m_Camera->GetViewProjectionMatrix() * model;

                m_LightCubeShader->SetUniformMat4f("u_MVP", m_MVP);
                m_LightCubeShader->SetUniform3f("u_Color", m_PointLightDiffuseColors[i]);

                renderer.Draw(*m_LightCubeVAO, *m_LightCubeIBO, *m_LightCubeShader);
            }

            model = glm::mat4(1.f);
            model = glm::translate(model, m_SpotLightPosition);
            model *= GetRotationMatrix(m_SpotLightRotation);
            model = glm::scale(model, m_LightCubeScale);
            m_MVP = m_Camera->GetViewProjectionMatrix() * model;

            m_LightCubeShader->SetUniformMat4f("u_MVP", m_MVP);
            m_LightCubeShader->SetUniform3f("u_Color", m_SpotLightDiffuseColor);

            renderer.Draw(*m_LightCubeVAO, *m_LightCubeIBO, *m_LightCubeShader);
        }

        // Draw cubes
        {
            m_CubeShader->Bind();

            // Update light information on cube shader
            m_CubeShader->SetUniform3f("u_DirLight.direction",GetForwardVector(m_DirectionalLightRotation));
            m_CubeShader->SetUniform3f("u_DirLight.ambient", m_DirectionalLightAmbientColor);
            m_CubeShader->SetUniform3f("u_DirLight.diffuse", m_DirectionalLightDiffuseColor);
            m_CubeShader->SetUniform3f("u_DirLight.specular", m_DirectionalLightSpecularColor);

            for(int i = 0; i < TOTAL_LIGHT_POINTS; i++)
            {
                m_CubeShader->SetUniform3f("u_PointLights[" + std::to_string(i) + "].position", m_PointLightPositions[i]);
                m_CubeShader->SetUniform1f("u_PointLights[" + std::to_string(i) + "].constant", m_PointLightConstants[i]);
                m_CubeShader->SetUniform1f("u_PointLights[" + std::to_string(i) + "].linear", m_PointLightLinears[i]);
                m_CubeShader->SetUniform1f("u_PointLights[" + std::to_string(i) + "].quadratic", m_PointLightQuadratics[i]);
                m_CubeShader->SetUniform3f("u_PointLights[" + std::to_string(i) + "].ambient", m_PointLightAmbientColors[i]);
                m_CubeShader->SetUniform3f("u_PointLights[" + std::to_string(i) + "].diffuse", m_PointLightDiffuseColors[i]);
                m_CubeShader->SetUniform3f("u_PointLights[" + std::to_string(i) + "].specular", m_PointLightSpecularColors[i]);
            }
            
            m_CubeShader->SetUniform3f("u_SpotLight.position", m_SpotLightPosition);
            m_CubeShader->SetUniform3f("u_SpotLight.direction", GetForwardVector(m_SpotLightRotation));
            m_CubeShader->SetUniform1f("u_SpotLight.cutoff", glm::cos(glm::radians(m_SpotLightCutoff))); // Pass the cos as it will be used in a dot product comparison
            m_CubeShader->SetUniform1f("u_SpotLight.outerCutoff", glm::cos(glm::radians(m_SpotLightOuterCutoff)));
            m_CubeShader->SetUniform3f("u_SpotLight.ambient", m_SpotLightAmbientColor);
            m_CubeShader->SetUniform3f("u_SpotLight.diffuse", m_SpotLightDiffuseColor);
            m_CubeShader->SetUniform3f("u_SpotLight.specular", m_SpotLightSpecularColor);

            // If we were calculating light in view space, we wouldn't need to pass the view position
            // since it would be 0,0,0
            m_CubeShader->SetUniform3f("u_ViewPosition", m_Camera->Position);
            
            // Update material properties
            m_CubeShader->SetUniform1f("u_Material.shininess", m_CubeShininess);
            
            for(int i = 0; i < TOTAL_CUBES; i++)
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

    glm::mat4 TestBasicLightingCasters::GetRotationMatrix(const glm::vec3& EulerRotation) const
    {
        glm::quat rotationQuat = glm::quat(glm::radians(EulerRotation));
        glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
        
        return rotationMatrix;
    }

    glm::vec3 TestBasicLightingCasters::GetForwardVector(const glm::vec3& EulerRotation) const
    {
        glm::quat rotationQuat = glm::quat(glm::radians(EulerRotation));
        glm::mat3 rotationMatrix = glm::mat3_cast(rotationQuat);
        glm::vec3 worldForward{0.f, 0.f, 1.f};
        glm::vec3 forward = glm::normalize(rotationMatrix * worldForward);

        return forward;
    }

    void TestBasicLightingCasters::OnImGuiRender()
    {
        // ImGui::Checkbox("Enable Orbit", &m_OrbitLightEnabled);

        if(ImGui::CollapsingHeader("Directional Light"))
        {
            ImGui::InputFloat3("Dir Position", &m_DirectionalLightPosition.x);
            ImGui::InputFloat3("Dir Rotation", &m_DirectionalLightRotation.x);

            ImGui::ColorEdit3("Dir Ambient", &m_DirectionalLightAmbientColor.x);
            ImGui::ColorEdit3("Dir Diffuse", &m_DirectionalLightDiffuseColor.x);
            ImGui::ColorEdit3("Dir Specular", &m_DirectionalLightSpecularColor.x);
        }
        
        if(ImGui::CollapsingHeader("Point Lights"))
        {
            for(int i = 0; i < TOTAL_LIGHT_POINTS; i++)
            {
                if(ImGui::CollapsingHeader(("Point Light" + std::to_string(i)).c_str()))
                {
                    ImGui::InputFloat3(("Position" + std::to_string(i)).c_str(), &m_PointLightPositions[i].x);
                    ImGui::InputFloat3(("Rotation" + std::to_string(i)).c_str(), &m_PointLightRotations[i].x);

                    ImGui::ColorEdit3(("Ambient" + std::to_string(i)).c_str(), &m_PointLightAmbientColors[i].x);
                    ImGui::ColorEdit3(("Diffuse" + std::to_string(i)).c_str(), &m_PointLightDiffuseColors[i].x);
                    ImGui::ColorEdit3(("Specular" + std::to_string(i)).c_str(), &m_PointLightSpecularColors[i].x);

                    ImGui::InputFloat(("Constant" + std::to_string(i)).c_str(), &m_PointLightConstants[i]);
                    ImGui::InputFloat(("Linear" + std::to_string(i)).c_str(), &m_PointLightLinears[i]);
                    ImGui::InputFloat(("Quadratic" + std::to_string(i)).c_str(), &m_PointLightQuadratics[i]);        
                }
            }

        }

        if(ImGui::CollapsingHeader("Spot Light"))
        {
            ImGui::InputFloat3("Spot Position", &m_SpotLightPosition.x);
            ImGui::InputFloat3("Spot Rotation", &m_SpotLightRotation.x);

            ImGui::ColorEdit3("Spot Ambient", &m_SpotLightAmbientColor.x);
            ImGui::ColorEdit3("Spot Diffuse", &m_SpotLightDiffuseColor.x);
            ImGui::ColorEdit3("Spot Specular", &m_SpotLightSpecularColor.x);
            
            ImGui::InputFloat("Cutoff", &m_SpotLightCutoff);
            ImGui::InputFloat("Outer Cutoff", &m_SpotLightOuterCutoff);
        }

        if(ImGui::CollapsingHeader("Cube"))
        {
            ImGui::InputFloat("C Shininess", &m_CubeShininess);
        }
        
        m_CameraController->OnImGuiRender();
    }
}
