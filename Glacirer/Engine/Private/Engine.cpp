#include "Engine.h"

#include "Application.h"
#include "GameTime.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Screen.h"

namespace
{
    void HandleWindowResized(GLFWwindow* Window, int Width, int Height)
    {
        // Don't update viewport size if window has been minimized
        // it causes an assert on glm perspective matrix calculation
        if(Width == 0 || Height == 0)
        {
            return;
        }

        glViewport(0, 0, Width, Height);
        Screen::SetSize(Width, Height);
    }
}

namespace Glacirer
{
    Engine::Engine()
    {
    }

    void Engine::Initialize(const char* windowTitle)
    {
        bool bSuccess = CreateWindow(windowTitle);
        assert(bSuccess);

        bSuccess = InitializeGlew();
        assert(bSuccess);

        bIsInitialized = bSuccess;
    }

    void Engine::Setup()
    {
        ResourceManager::LoadDefaultResources();

        m_World = std::make_unique<World>();
        m_World->Initialize();
        m_World->Setup();
    }

    void Engine::Shutdown()
    {
        glfwTerminate();

        m_Window = nullptr;

        m_World->Shutdown();
        m_World.reset();

        ResourceManager::UnloadAll();
    }

    bool Engine::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Engine::Update()
    {
        /* Swap front and back buffers */
        glfwSwapBuffers(m_Window);

        /* Poll for and process events, like keyboard input and mouse movement */
        Input::SwapBuffers();
        glfwPollEvents();
        Input::Process(m_Window);
        
        GameTime::Time = static_cast<float>(glfwGetTime());

        GameTime::DeltaTime = GameTime::Time - m_LastFrameTime;
        m_LastFrameTime = GameTime::Time;
        
        m_World->Update(GameTime::DeltaTime);
    }

    void Engine::Render()
    {
        m_World->Render();
    }

    bool Engine::CreateWindow(const char* windowTitle)
    {
        if (!glfwInit())
        {
            return false;
        }

        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4); // MSAA x4

        int windowWidth = 1280;
        int windowHeight = 720;

        // const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor()); 
        // const int INITIAL_WIDTH = vidMode->width;
        // const int INITIAL_HEIGHT = vidMode->height;
        /* Create a windowed mode window and its OpenGL context */
        m_Window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
        // window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "OpenGL Study", glfwGetPrimaryMonitor(), NULL); // if we want to create fullscreen
        if (!m_Window)
        {
            glfwTerminate();
            return false;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(0); // Synchronize with VSync (0 - disabled, 1 - enabled)
        Application::SetCurrentWindow(m_Window);

        glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);
        Screen::SetSize(windowWidth, windowHeight);
    
        glfwSetFramebufferSizeCallback(m_Window, HandleWindowResized); // Add callback for window resized

        return true;
    }

    bool Engine::InitializeGlew() const
    {
        return glewInit() == GLEW_OK;
    }
}
