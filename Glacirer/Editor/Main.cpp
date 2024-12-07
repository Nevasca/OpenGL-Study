#include <iostream>

#include "Engine.h"

int main()
{
    Glacirer::Engine engine{};
    engine.Initialize("Glacirer Engine");
    assert(engine.IsInitialized());
    engine.Setup();

    while(!engine.ShouldClose())
    {
        engine.Update();
        engine.Render();
    }
}
