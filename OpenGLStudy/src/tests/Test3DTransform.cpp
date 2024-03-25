#include "Test3DTransform.h"

#include "LegacyIndexBuffer.h"
#include "Renderer.h"
#include "LegacyShader.h"
#include "Texture.h"
#include "LegacyVertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glm/detail/type_quat.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include "imgui/imgui.h"

namespace tests
{
    Test3DTransform::Test3DTransform()
    {
        m_VAO = std::make_unique<LegacyVertexArray>();

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

        m_IBO = std::make_unique<LegacyIndexBuffer>(indices, 6 * 6);

        m_Shader = std::make_unique<LegacyShader>("res/shaders/Basic.shader");
        m_Texture = std::make_unique<Texture>("res/textures/FancyPigeon.png");
        
        m_Shader->Bind();
        m_Texture->Bind(0);
        m_Shader->SetUniform1i("u_Texture", 0);

        m_Proj = glm::perspective(glm::radians(45.f), 960.f / 540.f, 0.1f, 100.f);
        
        m_View = glm::mat4(1.f);
        m_View = glm::translate(m_View, glm::vec3(0.f, 0.f, -3.f));

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

    Test3DTransform::~Test3DTransform()
    { }

    void Test3DTransform::OnUpdate(float DeltaTime)
    {
        
    }

    glm::mat4 Test3DTransform::GetRotationMatrix(const glm::vec3& EulerRotation) const
    {
        glm::quat rotationQuat = glm::quat(glm::radians(EulerRotation));
        glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
        
        return rotationMatrix;
    }

    void Test3DTransform::OnRender()
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

    void Test3DTransform::OnImGuiRender()
    {
        for(int i = 0; i < m_TotalCubes; i++)
        {
            std::string readableIndex = std::to_string(i + 1);

            std::string title = "Cube " + readableIndex;
            ImGui::Text(title.c_str());

            // We need labels to be unique, otherwise editing one will edit all and we possibly won't be able to input a value  
            std::string positionLabel = "Position" + readableIndex;
            std::string scaleLabel = "Scale" + readableIndex;
            std::string rotationLabel = "Rotation" + readableIndex;
        
            ImGui::InputFloat3(positionLabel.c_str(), &m_Positions[i].x);
            ImGui::InputFloat3(scaleLabel.c_str(), &m_Scales[i].x);
            ImGui::InputFloat3(rotationLabel.c_str(), &m_Rotations[i].x);
        }
    }
}
