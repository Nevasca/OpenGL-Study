#include "Editor.h"

int main()
{
    GlacirerEditor::Editor editor{};
    editor.Initialize();
    assert(editor.IsInitialized());
    
    editor.Setup();

    while(!editor.ShouldClose())
    {
        editor.Update();
        editor.Render();
    }

    editor.Shutdown();

    return 0;
}
