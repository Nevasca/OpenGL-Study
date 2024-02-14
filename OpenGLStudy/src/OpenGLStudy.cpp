#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& FilePath)
{
    // This is the modern C++ approach of opening a file
    // In a serious game engine we should consider using the C approach, as it's faster than this
    std::ifstream Stream(FilePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string Line;
    std::stringstream Ss[2]; // One for the vertex shader [0] and other for the fragment [1]
    ShaderType Type = ShaderType::NONE;

    while(getline(Stream, Line))
    {
        // #shader is a custom token we are using to separate the shaders
        if(Line.find("#shader") != std::string::npos)
        {
            if(Line.find("vertex") != std::string::npos)
            {
                Type = ShaderType::VERTEX;
            }
            else if(Line.find("fragment") != std::string::npos)
            {
                Type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            Ss[(int)Type] << Line << "\n";
        }
    }

    return {Ss[0].str(), Ss[1].str()};
}

static unsigned int CompileShader(unsigned int Type, const std::string& Source)
{
    unsigned int Id = glCreateShader(Type);
    const char* Src = Source.c_str();

    // Shader Id
    // Number of source codes
    // Pointer to the C string source code
    // The length of source code in case we are providing multiples, nullptr if just one
    glShaderSource(Id, 1, &Src, nullptr);
    glCompileShader(Id);

    int result;
    glGetShaderiv(Id, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // To hack doing this to allocate on stack: char message[length];
        glGetShaderInfoLog(Id, length, &length, message);

        std::cout << "Failed to compile " << (Type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << "\n";

        // Delete shader as the compilation didn't work out
        glDeleteShader(Id);

        return 0;
    }
    
    return Id;
}

// Marked as static to don't appear on other translation units
// Takes source code of each shader and compile into shaders
static int CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
    // It's recommended to using the C++ type, like unsigned int, instead of the OpenGL defined
    // so when dealing with multiple APIs you don't need to include OpenGL on the header or so
    unsigned int Program = glCreateProgram();
    unsigned int Vs = CompileShader(GL_VERTEX_SHADER, VertexShader); 
    unsigned int Fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader); 

    // Link both shaders into the program 
    glAttachShader(Program, Vs);
    glAttachShader(Program, Fs);
    glLinkProgram(Program);
    glValidateProgram(Program);

    // Delete the intermediate shaders as they are now compiled into program
    glDeleteShader(Vs);
    glDeleteShader(Fs);

    return Program;
}

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
            -0.5f, -0.5f, // 0
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
            -0.5f,  0.5f, // 3
        };

        // We could use unsigned char, for instance,
        // but for more complex models it will require more bytes
        // but it must be an unsigned type
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray va{};

        VertexBuffer vb{positions, 4 * 2 * sizeof(float)};

        VertexBufferLayout layout;
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);

        IndexBuffer ib{indices, 6};

        // Tells how OpenGL should interpreted that data, it does not know yet they are vertex positions

        // Using OpenGL shader language version 330, core means to not let using deprecated functions
        // layout(location = 0) tells the incoming vec4 position uses the location 0 of the layout, the same index 0 we set on glVertexAttribPointer above
        // vec4 so OpenGL convert our two points (X,Y) into the vec4 expected by the gl_Position

        // Relative path from working directory. If running on a .exe, it will be the directory where it's running on.
        // For debugging mode on Rider, it will be whatever is set on Project Properties > Local Debugger (Debugging) > Working Directory (default set as ProjectDir)
        ShaderProgramSource Source = ParseShader("res/shaders/Basic.shader");
        unsigned shader = CreateShader(Source.VertexSource, Source.FragmentSource);

        glUseProgram(shader);

        // We call only call uniform when the shader is bound (glUseProgram)
        // Location: id of the uniform. When shader is created, every uniform is assigned to an id
        GLCall(int ColorUniformLocation = glGetUniformLocation(shader, "u_Color")); // name needs to exactly matches the one declared on .shader
        ASSERT(ColorUniformLocation != -1); // It algo might return -1 if the uniform is declared on source code but not used anywhere on the shader and the compiler has removed it
        //glUniform4f is to set uniform that receives 4 floats. There are other Uniform functions for other params 
        GLCall(glUniform4f(ColorUniformLocation, 0.5f, 0.0f, 0.5f, 1.0f));

        // Unbind all to test how vertex array works
        glUseProgram(0);
        va.Unbind();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
        float R = 0.0f;
        float Increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            // Approach for a draw call without an index buffer (using the current bound buffer (glBindBuffer)
            // Mode: GL_TRIANGLES
            // First element to start, in case we want to offset the data in selected buffer
            // How many vertex there are
            //glDrawArrays(GL_TRIANGLES, 0, 6);

            glUseProgram(shader);
            glUniform4f(ColorUniformLocation, R, 0.0f, 0.5f, 1.0f);

            va.Bind();
            ib.Bind();
        
            // Approach for a draw call with an index buffer
            // Mode: Triangles
            // How many indices there are
            // Type of the index on the index array
            // Pointer to the indices. Since we have used glBindBuffer(GL_ELEMENT_ARRAY_BUFFER... we don't need to specify one
            GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

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

        // Clean shader after usage so we don't leak on VRAM
        glDeleteProgram(shader);
    }

    glfwTerminate();
    return 0;
}