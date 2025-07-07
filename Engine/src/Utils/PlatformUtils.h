#pragma once
#include <optional>

namespace Engine
{
    class FileDialogs
    {
    public:
        static std::optional<std::string> OpenFile(const char* filter);
        static std::optional<std::string> SaveFile(const char* filter);
        static std::optional<std::string> SelectFolder();
    };
}
