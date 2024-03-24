#include "Test2DTransform.h"

#include "IndexBuffer.h"
#include "LegacyShader.h"
#include "Texture.h"
#include "GameTime.h"
#include "Renderer.h"
#include "VertexArray.h"
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
    Test2DTransform::Test2DTransform()
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

        m_Shader = std::make_unique<LegacyShader>("res/shaders/Basic.shader");
        m_Texture = std::make_unique<Texture>("res/textures/FancyPigeon.png");
        
        m_Shader->Bind();
        m_Texture->Bind(0);
        m_Shader->SetUniform1i("u_Texture", 0);

        m_Proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 100.f);
    }

    Test2DTransform::~Test2DTransform()
    { }

    void Test2DTransform::OnUpdate(float DeltaTime)
    {
        // Creates an identity matrix so we can start applying the transforms
        glm::mat4 model = glm::mat4(1.f);

        // Even though we are coding the translate, rotate and scale on this order
        // matrix operations should be read from right to left
        // so we are applying scale, then rotation and finally translation
        // if we don't do on this order, we will get wierd behaviour
        model = glm::translate(model, m_Position);

        model *= GetRotationMatrix();
        
        // glm expect angles in radians, we can use radians(degrees) for that. Rotates in the Z axis (0, 0, 1)
        // model = glm::rotate(model, glm::radians(45.f), glm::vec3(0.f, 0.f, 1.f));
        model = glm::rotate(model, GameTime::Time * m_RotationZSpeed, glm::vec3(0.f, 0.f, 1.f));
        
        model = glm::scale(model, m_Scale);
        
        glm::mat4 view = glm::mat4(1.f);

        m_MVP = m_Proj * view * model; // Once again, read from right to left 
    }

    glm::mat4 Test2DTransform::GetRotationMatrix() const
    {
        // Approach using quaternions to prevent gimbal lock, step by step -----------
        
        // // Convert Euler angles to radians
        // glm::vec3 rotation = glm::radians(m_Rotation);
        //
        // // Create quaternions for each axis
        // glm::quat quatX = glm::angleAxis(rotation.x, glm::vec3(1.f, 0.f, 0.f));
        // glm::quat quatY = glm::angleAxis(rotation.y, glm::vec3(0.f, 1.f, 0.f));
        // glm::quat quatZ = glm::angleAxis(rotation.z, glm::vec3(0.f, 0.f, 1.f));
        //
        // // Combine quaternions
        // glm::quat rotationQuat = quatX * quatY * quatZ;
        //
        // // Convert quaternion to rotation matrix
        // glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
        //
        // return rotationMatrix;

        // Approach using quaternions to prevent gimbal lock, shorter version ----------

        // Create quaternion rotation from euler angles converted to radians
        glm::quat rotationQuat = glm::quat(glm::radians(m_Rotation));

        // Convert quaternion to rotation matrix
        glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
        
        return rotationMatrix;

        // Approach using only euler angles (has gimbal lock) --------------------------
        
        // The simpler approach would just use the euler angles and apply the X, Y, Z rotations in this order
        // but this would cause gimbal lock

        // glm::vec3 rotation = glm::radians(m_Rotation);
        
        // glm::mat4 rotationMatrix = glm::mat4(1.f);
        // rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0.f, 0.f, 1.f));
        // rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0.f, 1.f, 0.f));
        // rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1.f, 0.f, 0.f));
        //
        // return rotationMatrix;
    }

    void Test2DTransform::OnRender()
    {
        Renderer renderer{};

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);
        
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }

    void Test2DTransform::OnImGuiRender()
    {
        ImGui::InputFloat3("Position", &m_Position.x);
        ImGui::InputFloat3("Scale", &m_Scale.x);
        ImGui::InputFloat3("Rotation", &m_Rotation.x);
        ImGui::InputFloat("Rotation Z Speed", &m_RotationZSpeed);
    }
}
