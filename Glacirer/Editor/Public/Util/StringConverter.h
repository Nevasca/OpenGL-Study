#pragma once
#include <string>

namespace GlacirerEditor
{
    namespace Util
    {
        class StringConverter
        {
        public:
            static std::string ConvertWideCharToString(const wchar_t* wString);
        };
    }
}
