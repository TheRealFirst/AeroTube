#include "atpch.h"
#include "Window.h"

#ifdef PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Engine
{
    Scope<Window> Window::Create(const WindowProbs& props)
    {
        #ifdef PLATFORM_WINDOWS
        return CreateScope<WindowsWindow>(props);
        #else
        AT_ASSERT(false, "Unknown platform!");
        return nullptr;
        #endif
    }
}