#include "Engine.h"

#include "Application.h"
#include "GameTime.h"
#include "Input.h"
#include "Resources/ResourceManager.h"
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
        Glacirer::Screen::SetSize(Width, Height);
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
        Resources::ResourceManager::LoadDefaultResources();

        m_RenderSystem = std::make_shared<Rendering::RenderSystem>(static_cast<unsigned int>(DEFAULT_MSAA_TOTAL_SAMPLES));
        m_RenderSystem->Setup();
        
        m_World = std::make_unique<World>();
        m_World->Initialize(m_RenderSystem);
        m_World->Setup();
    }

    void Engine::Shutdown()
    {
        glfwTerminate();

        m_Window = nullptr;

        m_World->Shutdown();
        m_World.reset();

        m_RenderSystem->Shutdown();
        m_RenderSystem.reset();

        Resources::ResourceManager::UnloadAll();
    }

    bool Engine::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Engine::Update()
    {
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
        /* Swap front and back buffers */
        glfwSwapBuffers(m_Window);

        std::shared_ptr<CameraComponent> activeCamera = m_World->GetActiveCamera();
        if(!activeCamera)
        {
            // TODO: Log warning (create a log class)
            return;
        }

        m_RenderSystem->Render(*activeCamera);
    }

    bool Engine::CreateWindow(const char* windowTitle)
    {
        if (!glfwInit())
        {
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        glfwWindowHint(GLFW_SAMPLES, DEFAULT_MSAA_TOTAL_SAMPLES);

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
        const GLenum result = glewInit();
        return result == GLEW_OK;
    }

    const char* Engine::GetGLSLVersion()
    {
        return "#version 330";
    }
    
}
