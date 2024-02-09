#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    glfwTerminate();
    return 0;
}