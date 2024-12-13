#pragma once
#include <string>

namespace GlacirerEditor
{
    namespace Util
    {
        class FileDialog
        {
        public:
            static bool OpenFile(const wchar_t* filter, std::string& outFilePath);
        };
    }
}
