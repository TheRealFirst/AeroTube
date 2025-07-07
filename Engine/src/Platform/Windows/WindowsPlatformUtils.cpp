#include "atpch.h"
#include "Utils/PlatformUtils.h"

#ifdef PLATFORM_WINDOWS

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Core/Application.h"

#include <ShObjIdl.h>

namespace Engine
{
    std::optional<std::string> FileDialogs::OpenFile(const char* filter)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = {0};
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if(GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		
        if (GetOpenFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;
        return std::nullopt;
    }

    std::optional<std::string> FileDialogs::SaveFile(const char* filter)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = {0};
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if(GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        // Sets the default extension by extracting it from the filter
        ofn.lpstrDefExt = strchr(filter, '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;
        return std::nullopt;
    }

    std::optional<std::string> FileDialogs::SelectFolder()
    {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED |
                                         COINIT_DISABLE_OLE1DDE);
        if (FAILED(hr))
            return std::nullopt;

        IFileDialog* pFileDialog = nullptr;
        hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
                              IID_PPV_ARGS(&pFileDialog));

        if (FAILED(hr)) {
            CoUninitialize();
            return std::nullopt;
        }

        // Set the dialog to be a folder picker
        DWORD options;
        pFileDialog->GetOptions(&options);
        pFileDialog->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

        HWND hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
        hr = pFileDialog->Show(hwndOwner);
        if (SUCCEEDED(hr)) {
            IShellItem* pItem;
            hr = pFileDialog->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR pszFilePath = nullptr;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                std::optional<std::string> result;
                if (SUCCEEDED(hr)) {
                    // Convert from wide string to std::string
                    char path[MAX_PATH];
                    WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, path, MAX_PATH, nullptr, nullptr);
                    result = std::string(path);
                    CoTaskMemFree(pszFilePath);
                }
                pItem->Release();
                pFileDialog->Release();
                CoUninitialize();
                return result;
            }
        }

        if (pFileDialog) pFileDialog->Release();
        CoUninitialize();
        return std::nullopt;
    }
}
#endif