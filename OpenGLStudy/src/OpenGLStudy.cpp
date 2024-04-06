#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>

#include "Renderer.h"
#include "GameTime.h"
#include "core/Application.h"
#include "core/Input.h"
#include "core/Screen.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "tests/TestStaticBatchRendering.h"
#include "tests/TestClearColor.h"
#include "tests/TestDynamicBatchRendering.h"
#include "tests/TestMenu.h"
#include "tests/TestTexture2D.h"
#include "tests/Test2DTransform.h"
#include "tests/Test3DTransform.h"
#include "tests/TestCamera.h"
#include "tests/TestBasicLighting.h"
#include "tests/TestBasicLightingCasters.h"
#include "tests/testCore/TestCore.h"
#include "tests/TestFramebuffer.h"
#include "tests/TestGameBreakout.h"
#include "tests/TestModelLoading.h"
#include "tests/TestTextRendering.h"

namespace
{
    tests::Test* currentTest = nullptr;
    tests::TestMenu* testMenu = nullptr;
}

void HandleWindowResized(GLFWwindow* Window, int Width, int Height)
{
    std::cout << "Window has been resized to (" << Width << ", " << Height << ")\n";
    
    // Update the Projection matrix here, or use glViewport(0, 0, Width, Height) if not using matrices
    glViewport(0, 0, Width, Height);
    Screen::SetSize(Width, Height);
}

void ProcessInput(GLFWwindow* Window)
{
    // Request close when ESC and is on main menu
    if(glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS && currentTest == testMenu) // returns GLFW_RELEASE if not pressed
    {
        glfwSetWindowShouldClose(Window, true);
    }
}

void CreateTests()
{
    testMenu->RegisterTest<tests::TestClearColor>("Clear Color");
    testMenu->RegisterTest<tests::TestTexture2D>("2D Texture");
    testMenu->RegisterTest<tests::TestStaticBatchRendering>("Static Batch Rendering");
    testMenu->RegisterTest<tests::TestDynamicBatchRendering>("Dynamic Batch Rendering");
    testMenu->RegisterTest<tests::Test2DTransform>("2D Transform");
    testMenu->RegisterTest<tests::Test3DTransform>("3D Transform");
    testMenu->RegisterTest<tests::TestCamera>("Camera");
    testMenu->RegisterTest<tests::TestBasicLighting>("Basic Lighting");
    testMenu->RegisterTest<tests::TestBasicLightingCasters>("Basic Lighting Casters");
    testMenu->RegisterTest<tests::TestTextRendering>("Text Rendering");
    testMenu->RegisterTest<tests::TestModelLoading>("Model Loading");
    testMenu->RegisterTest<tests::TestFramebuffer>("Framebuffer");
    testMenu->RegisterTest<tests::TestGameBreakout>("Breakout Game");
    testMenu->RegisterTest<tests::TestCore>("Core");
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

    constexpr int INITIAL_WIDTH = 1280;
    constexpr int INITIAL_HEIGHT = 720;

    // const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor()); 
    // const int INITIAL_WIDTH = vidMode->width;
    // const int INITIAL_HEIGHT = vidMode->height;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "OpenGL Study", NULL, NULL);
    // window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "OpenGL Study", glfwGetPrimaryMonitor(), NULL); // if we want to create fullscreen
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Synchronize with VSync (0 - disabled, 1 - enabled)
    Application::SetCurrentWindow(window);
    glViewport(0, 0, INITIAL_WIDTH, INITIAL_HEIGHT);
    Screen::SetSize(INITIAL_WIDTH, INITIAL_HEIGHT);
    
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

        // Enables depth test so OpenGL can proper decide to draw or not a pixel on top of existing one when using perspective
        GLCall(glEnable(GL_DEPTH_TEST));

        Renderer renderer{};
        
        // Setup ImGUI
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        currentTest = nullptr;
        testMenu = new tests::TestMenu{currentTest, *window};

        // Starts with menu,
        // we could add a argument to boot application with an specific test as well
        // to spare going through the menu every time testing something new
        currentTest = testMenu;

        CreateTests();

        // testMenu->OpenTest("Breakout Game");

        float lastFrameTime = 0.f;
        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GameTime::Time = static_cast<float>(glfwGetTime());

            GameTime::DeltaTime = GameTime::Time - lastFrameTime;
            lastFrameTime = GameTime::Time;            
            
            ProcessInput(window);

            GLCall(glClearColor(0.f, 0.f, 0.f, 1.f)); // Sets clear color to black
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if(currentTest)
            {
                currentTest->OnUpdate(GameTime::DeltaTime);
                currentTest->OnRender();

                ImGui::Begin("Test");

                // Return to menu if back button clicked
                if(currentTest != testMenu && ImGui::Button("<-"))
                {
                    currentTest->Shutdown(window);
                    delete currentTest;
                    currentTest = testMenu;
                }

                currentTest->OnImGuiRender();
                ImGui::End();
            }

            {
                ImGui::Begin("Statistics");
                ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events, like keyboard input and mouse movement */
            Input::SwapBuffers();
            glfwPollEvents();
            Input::Process(window);
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
