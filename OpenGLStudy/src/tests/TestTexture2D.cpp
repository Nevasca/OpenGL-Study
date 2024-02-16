#include "TestTexture2D.h"

#include "VertexBufferLayout.h"
#include "imgui/imgui.h"

namespace tests
{
    TestTexture2D::TestTexture2D()
    {
        // Set OpenGL behaviour for handling transparency and enable it
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        float vertexData[16] {
            -50.f, -50.f, 0.0f, 0.0f, // 0 // Bottom left vertex
             50.f, -50.f, 1.0f, 0.0f, // 1 // Bottom right
             50.f,  50.f, 1.0f, 1.0f, // 2 // Top right
            -50.f,  50.f, 0.0f, 1.0f  // 3 // Top left
        };

        // We could use unsigned char, for instance,
        // but for more complex models it will require more bytes
        // but it must be an unsigned type
        unsigned int indices[6] {
            0, 1, 2,
            2, 3, 0
        };

        m_VertexArray = std::make_unique<VertexArray>();

        m_VertexBuffer = std::make_unique<VertexBuffer>(vertexData, 4 * 4 * sizeof(float));
        VertexBufferLayout layout{};
        layout.Push<float>(2); // First 2 floats are positions
        layout.Push<float>(2); // Next 2 floats are UV (texture coordinates)

        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        // Tells how OpenGL should interpreted that data, it does not know yet they are vertex positions

        // Using OpenGL shader language version 330, core means to not let using deprecated functions
        // layout(location = 0) tells the incoming vec4 position uses the location 0 of the layout, the same index 0 we set on glVertexAttribPointer above
        // vec4 so OpenGL convert our two points (X,Y) into the vec4 expected by the gl_Position

        // Relative path from working directory. If running on a .exe, it will be the directory where it's running on.
        // For debugging mode on Rider, it will be whatever is set on Project Properties > Local Debugger (Debugging) > Working Directory (default set as ProjectDir)
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader"); // Initialized on member initialization list

        // We call only call uniform when the shader is bound (glUseProgram)
        m_Shader->Bind();
        // const std::string ColorUniformName = "u_Color";
        // shader.SetUniform4f(ColorUniformName, 0.5f, 0.0f, 0.5f, 1.0f);

        m_Texture = std::make_unique<Texture>("res/textures/FancyPigeon.png");
        m_Texture->Bind();
        // Set the uniform with the slot our texture sampler is
        // The value needs to be the same slot we bound the texture to, if we had texture.Bind(2), this would be 2
        m_Shader->SetUniform1i("u_Texture", 0);

        // Create a mat 4x4 with 4:3 aspect ratio (left edge, right edge...)
        // On orthographic projection, objects that are far away don't get smaller, opposed to a perspective projection
        // glm::mat4 proj = glm::ortho(-2.f, 2.f, -1.5f, 1.5f, -1.f, 1.f);
        m_Proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f); // Here we define we will work with pixels values

        // Creates a identity matrix (glm::mat4{1.f}), translate it to the left in 100 units and set as the view,
        // The 'camera' here is positioned a little to the right, so we need to do the inverse so we move objects to the left, therefore the -100.f instead of 100.f
        // In other words, moving the camera to the right means moving all objects to the left
        // If we had a Camera class, we would automate this negation
        // glm::mat4 view = glm::translate(glm::mat4{1.f}, glm::vec3{-100.f, 0.f, 0.f});
        m_View = glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 0.f, 0.f});
    }

    TestTexture2D::~TestTexture2D()
    { }

    void TestTexture2D::OnUpdate(float DeltaTime)
    { }

    void TestTexture2D::OnRender()
    {
        // Since our renderer class has no members, we can allocate it on stack every time we need it, almost free
        Renderer renderer{};
        
        // Draws first object
        {
            // Creates the model matrix
            glm::mat4 model = glm::translate(glm::mat4{1.f}, m_translationA);
            glm::mat4 mvp = m_Proj * m_View * model; // Creates the MVP, in OpenGL we multiply it on reverse order
                
            m_Shader->Bind();
            // shader.SetUniform4f(ColorUniformName, R, 0.0f, 0.5f, 1.0f);
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }

        // Draws second object
        {
            glm::mat4 model = glm::translate(glm::mat4{1.f}, m_translationB);
            glm::mat4 mvp = m_Proj * m_View * model; // Creates the MVP, in OpenGL we multiply it on reverse order

            // Redundant second bind, since is already bound and will cost some performance
            // In a more robust engine we would have something to check that is already bound and ignore the call
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
