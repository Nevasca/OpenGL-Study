#include "Util/StringConverter.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace GlacirerEditor
{
    namespace Util
    {
        std::string StringConverter::ConvertWideCharToString(const wchar_t* wString)
        {
            const int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wString, -1, nullptr, 0, nullptr, nullptr);
            std::string convertedString(bufferSize, 0);
            WideCharToMultiByte(CP_UTF8, 0, wString, -1, &convertedString[0], bufferSize, nullptr, nullptr);

            return convertedString;
        }
    }
}
