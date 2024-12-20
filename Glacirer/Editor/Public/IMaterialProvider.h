#pragma once
#include <string>

namespace GlacirerEditor
{
    class IMaterialProvider
    {
    public:
        virtual ~IMaterialProvider() = default;

        virtual bool HasAnyMaterialSelected() const = 0;
        virtual std::string GetCurrentSelectedMaterialName() const = 0;

    protected:
        IMaterialProvider() = default;
    };
}
