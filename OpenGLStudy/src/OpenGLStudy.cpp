#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Synchronize with VSync

    if(glewInit() != GLEW_OK)
    {
        std::cout << "Error initiating GLEW";
    }

    std::cout << glGetString(GL_VERSION) << '\n';

    { // Creating a new scope so the stack allocated vertex and index buffers can proper destroy themselves before the OpenGL context is destroyed (glfwTerminate)
        float positions[] = {
            100.f, 100.f, 0.0f, 0.0f, // 0 // Bottom left vertex
            200.f, 100.f, 1.0f, 0.0f, // 1 // Bottom right
            200.f, 200.f, 1.0f, 1.0f, // 2 // Top right
            100.f, 200.f, 0.0f, 1.0f  // 3 // Top left
        };

        // We could use unsigned char, for instance,
        // but for more complex models it will require more bytes
        // but it must be an unsigned type
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Set OpenGL behaviour for handling transparency and enable it
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); 
        
        VertexArray va{};

        VertexBuffer vb{positions, 4 * 4 * sizeof(float)};

        VertexBufferLayout layout;
        layout.Push<float>(2); // First 2 floats are positions
        layout.Push<float>(2); // Next 2 floats are UV (texture coordinates)

        va.AddBuffer(vb, layout);

        IndexBuffer ib{indices, 6};

        // Create a mat 4x4 with 4:3 aspect ratio (left edge, right edge...)
        // On orthographic projection, objects that are far away don't get smaller, opposed to a perspective projection
        // glm::mat4 proj = glm::ortho(-2.f, 2.f, -1.5f, 1.5f, -1.f, 1.f);
        glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f); // Here we define we will work with pixels values
        
        // Creates a identity matrix (glm::mat4{1.f}), translate it to the left in 100 units and set as the view,
        // The 'camera' here is positioned a little to the right, so we need to do the inverse so we move objects to the left, therefore the -100.f instead of 100.f
        // In other words, moving the camera to the right means moving all objects to the left
        // If we had a Camera class, we would automate this negation
        glm::mat4 view = glm::translate(glm::mat4{1.f}, glm::vec3{-100.f, 0.f, 0.f});
        
        // Tells how OpenGL should interpreted that data, it does not know yet they are vertex positions

        // Using OpenGL shader language version 330, core means to not let using deprecated functions
        // layout(location = 0) tells the incoming vec4 position uses the location 0 of the layout, the same index 0 we set on glVertexAttribPointer above
        // vec4 so OpenGL convert our two points (X,Y) into the vec4 expected by the gl_Position

        // Relative path from working directory. If running on a .exe, it will be the directory where it's running on.
        // For debugging mode on Rider, it will be whatever is set on Project Properties > Local Debugger (Debugging) > Working Directory (default set as ProjectDir)
        Shader shader{"res/shaders/Basic.shader"};

        // We call only call uniform when the shader is bound (glUseProgram)
        shader.Bind();
        const std::string ColorUniformName = "u_Color";
        shader.SetUniform4f(ColorUniformName, 0.5f, 0.0f, 0.5f, 1.0f);

        Texture texture{"res/textures/FancyPigeon.png"};
        texture.Bind();
        // Set the uniform with the slot our texture sampler is
        // The value needs to be the same slot we bound the texture to, if we had texture.Bind(2), this would be 2
        shader.SetUniform1i("u_Texture", 0); 
        
        Renderer renderer{};

        // Setup ImGUI
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        glm::vec3 translation = glm::vec3{200.f, 200.f, 0.f};
        
        // Unbind all to test how vertex array works
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();
    
        float R = 0.0f;
        float Increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Creates the model matrix
            glm::mat4 model = glm::translate(glm::mat4{1.f}, translation);
            glm::mat4 mvp = proj * view * model; // Creates the MVP, in OpenGL we multiply it on reverse order
            
            shader.Bind();
            shader.SetUniform4f(ColorUniformName, R, 0.0f, 0.5f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);

            if(R > 1.0f)
            {
                Increment = -0.05f;
            }
            else if(R < 0.0f)
            {
                Increment = 0.05f;
            }

            R += Increment;

            {
                ImGui::Begin("OpenGL Study");                          // Create a window called "Hello, world!" and append into it.
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
