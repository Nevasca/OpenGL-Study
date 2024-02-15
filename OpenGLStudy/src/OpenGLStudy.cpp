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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
            -0.5f, -0.5f, 0.0f, 0.0f, // 0 // Bottom left vertex
             0.5f, -0.5f, 1.0f, 0.0f, // 1 // Bottom right
             0.5f,  0.5f, 1.0f, 1.0f, // 2 // Top right
            -0.5f,  0.5f, 0.0f, 1.0f  // 3 // Top left
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

            shader.Bind();
            shader.SetUniform4f(ColorUniformName, R, 0.0f, 0.5f, 1.0f);

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
        
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}