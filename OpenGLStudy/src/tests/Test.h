#pragma once

class Renderer;

namespace tests
{
    class Test
    {
    public:

        virtual ~Test() = default;

        virtual void OnUpdate(float DeltaTime);
        virtual void OnRender(const Renderer& Renderer);
        virtual void OnImGuiRender();
    };
}
