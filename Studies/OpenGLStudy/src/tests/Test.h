#pragma once

class Renderer;
struct GLFWwindow;

namespace tests
{
    class Test
    {
    public:

        virtual ~Test() = default;

        virtual void Setup(GLFWwindow* Window);
        virtual void OnUpdate(float DeltaTime);
        virtual void OnRender();
        virtual void OnImGuiRender();
        virtual void Shutdown(GLFWwindow* Window);
    };
}
