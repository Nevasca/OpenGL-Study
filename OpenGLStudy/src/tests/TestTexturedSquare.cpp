#include "TestTexturedSquare.h"

#include "imgui/imgui.h"

namespace tests
{
    TestTexturedSquare::TestTexturedSquare()
        : vb(positions, 4 * 4 * sizeof(float)),
        ib(indices, 6),
        shader("res/shaders/Basic.shader"),
        texture("res/textures/FancyPigeon.png")
    {
        VertexBufferLayout layout{};
        layout.Push<float>(2); // First 2 floats are positions
        layout.Push<float>(2); // Next 2 floats are UV (texture coordinates)

        va.AddBuffer(vb, layout);

        // Create a mat 4x4 with 4:3 aspect ratio (left edge, right edge...)
        // On orthographic projection, objects that are far away don't get smaller, opposed to a perspective projection
        // glm::mat4 proj = glm::ortho(-2.f, 2.f, -1.5f, 1.5f, -1.f, 1.f);
        proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f); // Here we define we will work with pixels values

        // Creates a identity matrix (glm::mat4{1.f}), translate it to the left in 100 units and set as the view,
        // The 'camera' here is positioned a little to the right, so we need to do the inverse so we move objects to the left, therefore the -100.f instead of 100.f
        // In other words, moving the camera to the right means moving all objects to the left
        // If we had a Camera class, we would automate this negation
        // glm::mat4 view = glm::translate(glm::mat4{1.f}, glm::vec3{-100.f, 0.f, 0.f});
        view = glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 0.f, 0.f});

        // Tells how OpenGL should interpreted that data, it does not know yet they are vertex positions

        // Using OpenGL shader language version 330, core means to not let using deprecated functions
        // layout(location = 0) tells the incoming vec4 position uses the location 0 of the layout, the same index 0 we set on glVertexAttribPointer above
        // vec4 so OpenGL convert our two points (X,Y) into the vec4 expected by the gl_Position

        // Relative path from working directory. If running on a .exe, it will be the directory where it's running on.
        // For debugging mode on Rider, it will be whatever is set on Project Properties > Local Debugger (Debugging) > Working Directory (default set as ProjectDir)
        // shader = {"res/shaders/Basic.shader"}; // Initialized on member initialization list

        // We call only call uniform when the shader is bound (glUseProgram)
        shader.Bind();
        // const std::string ColorUniformName = "u_Color";
        // shader.SetUniform4f(ColorUniformName, 0.5f, 0.0f, 0.5f, 1.0f);

        // texture = {"res/textures/FancyPigeon.png"};
        texture.Bind();
        // Set the uniform with the slot our texture sampler is
        // The value needs to be the same slot we bound the texture to, if we had texture.Bind(2), this would be 2
        shader.SetUniform1i("u_Texture", 0); 
    }

    TestTexturedSquare::~TestTexturedSquare()
    { }

    void TestTexturedSquare::OnUpdate(float DeltaTime)
    { }

    void TestTexturedSquare::OnRender(const Renderer& Renderer)
    {
        // Unbind all to test how vertex array works
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        // Draws first object
        {
            // Creates the model matrix
            glm::mat4 model = glm::translate(glm::mat4{1.f}, translationA);
            glm::mat4 mvp = proj * view * model; // Creates the MVP, in OpenGL we multiply it on reverse order
                
            shader.Bind();
            // shader.SetUniform4f(ColorUniformName, R, 0.0f, 0.5f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);

            Renderer.Draw(va, ib, shader);
        }

        // Draws second object
        {
            glm::mat4 model = glm::translate(glm::mat4{1.f}, translationB);
            glm::mat4 mvp = proj * view * model; // Creates the MVP, in OpenGL we multiply it on reverse order

            // Redundant second bind, since is already bound and will cost some performance
            // In a more robust engine we would have something to check that is already bound and ignore the call
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);

            Renderer.Draw(va, ib, shader);
        }
    }

    void TestTexturedSquare::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
    }
}
