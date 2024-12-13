#include "Util/FileDialog.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>   // for glfwGetWin32Window()
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commdlg.h>

#include "Application.h"
#include "Util/StringConverter.h"

namespace GlacirerEditor
{
    namespace Util
    {
        bool FileDialog::OpenFile(const wchar_t* filter, std::string& outFilePath)
        {
            wchar_t fileName[256];
            OPENFILENAME fileSettings = {};
            fileSettings.lStructSize = sizeof(fileSettings);
            fileSettings.hwndOwner = glfwGetWin32Window(Glacirer::Application::GetCurrentWindow());
            fileSettings.lpstrFile = fileName;
            fileSettings.lpstrFile[0] = '\0';
            fileSettings.nMaxFile = sizeof(fileName);
            fileSettings.lpstrFilter = filter;
            fileSettings.nFilterIndex = 1;
            fileSettings.lpstrTitle = nullptr;
            fileSettings.nMaxFileTitle = 0;
            fileSettings.lpstrInitialDir = nullptr;
            fileSettings.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if(GetOpenFileName(&fileSettings))
            {
                outFilePath = StringConverter::ConvertWideCharToString(fileSettings.lpstrFile);
                return true;
            }

            outFilePath = std::string{};
            return false;
        }
    }
}
