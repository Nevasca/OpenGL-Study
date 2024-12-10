#pragma once

#include <memory>
#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include "EngineAPI.h"
#include "World.h"

namespace Glacirer
{
    class ENGINE_API Engine
    {
    public:
        Engine();

        void Initialize(const char* windowTitle);
        void Setup();
        void Update();
        void Render();
        void Shutdown();

        World& GetWorld() const { return *m_World; }
        GLFWwindow* GetWindow() const { return m_Window; }

        bool IsInitialized() const { return bIsInitialized; }
        bool ShouldClose() const;

        static const char* GetGLSLVersion();

    private:

        static constexpr int DEFAULT_MSAA_TOTAL_SAMPLES = 4;

        GLFWwindow* m_Window{nullptr};
        std::unique_ptr<World> m_World{};
        std::shared_ptr<Rendering::RenderSystem> m_RenderSystem{};
        bool bIsInitialized{false};
        float m_LastFrameTime{0.f};

        bool CreateWindow(const char* windowTitle);
        bool InitializeGlew() const;
    };
}
