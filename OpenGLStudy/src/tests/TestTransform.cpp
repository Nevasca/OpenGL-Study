#include "TestTransform.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "GameTime.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace tests
{
    TestTransform::TestTransform()
    {
        m_VAO = std::make_unique<VertexArray>();

        // Transforms will be applying using the origin of the object
        // if we want to rotate the quad from the center, it's origin needs to be the center as well
        float vertices[]
        {
            // Position      // UV
             -50.f, -50.f,   0.f, 0.f,
              50.f, -50.f,   1.f, 0.f,
              50.f,  50.f,   1.f, 1.f,
             -50.f,  50.f,   0.f, 1.f
        };

        unsigned int indices[]
        {
            0, 1, 2,
            2, 3, 0
        };

        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices), false);

        VertexBufferLayout layout{};
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VBO, layout);

        m_IBO = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Texture = std::make_unique<Texture>("res/textures/FancyPigeon.png");
        
        m_Shader->Bind();
        m_Texture->Bind(0);
        m_Shader->SetUniform1i("u_Texture", 0);

        m_Proj = glm::ortho(0.f, 960.f, 0.f, 540.f);
    }

    TestTransform::~TestTransform()
    { }

    void TestTransform::OnUpdate(float DeltaTime)
    {
        // Creates an identity matrix so we can start applying the transforms
        glm::mat4 model = glm::mat4(1.f);

        // Even though we are coding the translate, rotate and scale on this order
        // matrix operations should be read from right to left
        // so we are applying scale, then rotation and finally translation
        // if we don't do on this order, we will get wierd behaviour
        model = glm::translate(model, m_Position);

        // glm expect angles in radians, we can use radians(degrees) for that. Rotates in the Z axis (0, 0, 1)
        // model = glm::rotate(model, glm::radians(45.f), glm::vec3(0.f, 0.f, 1.f));
        model = glm::rotate(model, GameTime::Time * m_RotationSpeed, glm::vec3(0.f, 0.f, 1.f));
        
        model = glm::scale(model, m_Scale);
        
        glm::mat4 view = glm::mat4(1.f);

        m_MVP = m_Proj * view * model; // Once again, read from right to left 
    }

    void TestTransform::OnRender()
    {
        Renderer renderer{};

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);
        
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }

    void TestTransform::OnImGuiRender()
    {
        ImGui::InputFloat3("Position", &m_Position.x);
        ImGui::InputFloat3("Scale", &m_Scale.x);
        ImGui::InputFloat("Rotation Speed", &m_RotationSpeed);
    }
}
