#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>

#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "tests/TestStaticBatchRendering.h"
#include "tests/TestClearColor.h"
#include "tests/TestDynamicBatchRendering.h"
#include "tests/TestMenu.h"
#include "tests/TestTexture2D.h"

void HandleWindowResized(GLFWwindow* Window, int Width, int Height)
{
    std::cout << "Window has been resized to (" << Width << ", " << Height << ")\n";
    
    // Update the Projection matrix here, or use glViewport(0, 0, Width, Height) if not using matrices
}

void ProcessInput(GLFWwindow* Window)
{
    if(glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // returns GLFW_RELEASE if not pressed
    {
        glfwSetWindowShouldClose(Window, true);
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "OpenGL Study", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Synchronize with VSync
    
    glfwSetFramebufferSizeCallback(window, HandleWindowResized); // Add callback for window resized

    if(glewInit() != GLEW_OK)
    {
        std::cout << "Error initiating GLEW";
    }

    std::cout << glGetString(GL_VERSION) << '\n';
    int maxVertexAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
    std::cout << "Max vertex attributes supported: " << maxVertexAttributes << "\n";

    { // Creating a new scope so the stack allocated vertex and index buffers can proper destroy themselves before the OpenGL context is destroyed (glfwTerminate)

        // Set OpenGL behaviour for handling transparency and enable it
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer{};
        
        // Setup ImGUI
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        tests::Test* currentTest = nullptr;
        tests::TestMenu* testMenu = new tests::TestMenu{currentTest};

        // Starts with menu,
        // we could add a argument to boot application with an specific test as well
        // to spare going through the menu every time testing something new
        currentTest = testMenu;

        testMenu->RegisterTest<tests::TestClearColor>("Clear Color");
        testMenu->RegisterTest<tests::TestTexture2D>("2D Texture");
        testMenu->RegisterTest<tests::TestStaticBatchRendering>("Static Batch Rendering");
        testMenu->RegisterTest<tests::TestDynamicBatchRendering>("Dynamic Batch Rendering");
        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            ProcessInput(window);

            GLCall(glClearColor(0.f, 0.f, 0.f, 1.f)); // Sets clear color to black
            renderer.Clear();

            float time = static_cast<float>(glfwGetTime());

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if(currentTest)
            {
                currentTest->OnUpdate(0.f);
                currentTest->OnRender();

                ImGui::Begin("Test");

                // Return to menu if back button clicked
                if(currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }

                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events, like keyboard input and mouse movement */
            glfwPollEvents();
        }

        delete currentTest;

        if(currentTest != testMenu)
        {
            delete testMenu;
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
