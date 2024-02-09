#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        std::cout << "Error initiating GLEW";
    }

    std::cout << glGetString(GL_VERSION) << '\n';

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };
    
    unsigned int buffer; // Will store the generated buffer id (an object that can be a vertex buffer, vertex array, a texture, shader...)
    glGenBuffers(1, &buffer); // How many buffers to generate (1) and pointer to set the generated buffer id (&buffer)
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // Select the buffer to use with how it`s going to be used
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // Static means it will be modified once and used many times, and to draw

    // glBindBuffer(GL_ARRAY_BUFFER, 0); // Select no buffer

    // Enables the vertex attribute of index 0, could be called after the attribute call as well, as long as it's called after bind
    // If we don't enable it, the attribute won't work
    glEnableVertexAttribArray(0); 

    // Define the layout of the buffer
    // index: index of the attribute we are specifying
    // size: count of the types it has
    // type: what type of data it is
    // normalized: should we normalize it to 0-1?
    // offset: bytes offset to next vertex
    // pointer: bytes offset on a single vertex where this attribute starts on (if not zero, we would need to use (const void*)8 for instance)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    // Tells how OpenGL should interpreted that data, it does not know yet they are vertex positions

    // R"myDelim()myDelim" is a C++ 11 way of defining a multiline string, instead of using \n on each line and concatenating them
    
    // Using OpenGL shader language version 330, core means to not let using deprecated functions
    // layout(location = 0) tells the incoming vec4 position uses the location 0 of the layout, the same index 0 we set on glVertexAttribPointer above
    // vec4 so OpenGL convert our two points (X,Y) into the vec4 expected by the gl_Position
    std::string VertexShader = R"glsl(
        #version 330 core

        layout(location = 0) in vec4 position;        

        void main()
        {
            gl_Position = position;
        }
    )glsl";

    // We don't need to leave the layout definition here, it's optional for the out (maybe if it's going to be used for a next shader pass?)
    // out vec4 Color is RGBA, normalized (between 0 and 1)
    std::string FragmentShader = R"glsl(
        #version 330 core

        layout(location = 0) out vec4 color;        

        void main()
        {            
            color = vec4(0.5, 0.0, 0.5, 1.0);
        }
    )glsl";
    
        
    unsigned shader = CreateShader(VertexShader, FragmentShader);
    glUseProgram(shader);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Approach for a draw call without an index buffer (using the current bound buffer (glBindBuffer)
        // Mode: GL_TRIANGLES
        // First element to start, in case we want to offset the data in selected buffer
        // How many vertex there are
        glDrawArrays(GL_TRIANGLES, 0, 3); 

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean shader after usage so we don't leak on VRAM
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}