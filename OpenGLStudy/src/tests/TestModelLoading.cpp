#include "TestModelLoading.h"

#include <memory>

#include "LegacyCamera.h"
#include "FlyCameraController.h"
#include "Model.h"
#include "Shader.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace tests
{
    TestModelLoading::TestModelLoading()
    {
        m_Camera = std::make_shared<LegacyCamera>(960.f, 540.f);
        m_Camera->Position = {10.f, 10.f,15.f};

        m_CameraController = std::make_unique<FlyCameraController>(m_Camera);

        m_Shader = std::make_unique<Shader>("res/shaders/BasicLitNoCustomColors.shader");
        m_Shader->Bind();

        LoadModel();
    }

    TestModelLoading::~TestModelLoading()
    {
    }

    void TestModelLoading::Setup(GLFWwindow* Window)
    {
        m_CameraController->Setup();
    }

    void TestModelLoading::Shutdown(GLFWwindow* Window)
    {
        m_CameraController->Shutdown();
    }

    void TestModelLoading::OnUpdate(float DeltaTime)
    {
        m_CameraController->Update();
    }

    void TestModelLoading::OnRender()
    {
        if(!m_Model)
        {
            return;
        }

        glm::mat4 modelMatrix = glm::mat4{1.f};
        modelMatrix = glm::scale(modelMatrix, m_Scale);
        
        m_MVP = m_Camera->GetViewProjectionMatrix() * modelMatrix;
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);
        m_Shader->SetUniformMat4f("u_Model", modelMatrix);
        m_Shader->SetUniform3f("u_ViewPosition", m_Camera->Position);
        m_Shader->SetUniform3f("u_ObjectColor", glm::vec3{0.5});
        m_Shader->SetUniform3f("u_LightColor", glm::vec3{1.f});
        m_Shader->SetUniform3f("u_LightPosition", glm::vec3{0.f, 4.f, 4.f});
        
        m_Model->Draw(*m_Shader);
    }

    void TestModelLoading::OnImGuiRender()
    {
        ImGui::Text(m_ModelFilePath.c_str());
        ImGui::InputFloat3("Scale", &m_Scale.x);
    }

    void TestModelLoading::LoadModel()
    {
        m_Model.reset();
        m_Model = std::make_unique<Model>(m_ModelFilePath);
    }
}
