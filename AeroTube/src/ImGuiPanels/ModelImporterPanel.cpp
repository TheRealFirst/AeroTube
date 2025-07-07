#include "ModelImporterPanel.h"

#include "imgui/imgui.h"
#include "Utils/PlatformUtils.h"

static char buffer[128] = "";

void ModelImporterPanel::OnImGuiRender()
{
    if(m_IsRunning)
    {
        ImGui::Begin("Model loader", &m_IsRunning);

        
        ImGui::InputText("Name", buffer, sizeof(buffer));

        // Update name only when valid input is present
        if (strlen(buffer) > 0)
        {
            m_Name = buffer;
        }

        if(ImGui::Button("Save Model"))
        {
            
            std::optional<std::string> filepath = Engine::FileDialogs::SelectFolder();
            if (filepath)
            {
                m_Importer.SaveModel(*filepath, m_Name);
                memset(buffer, 0, sizeof(buffer));
                m_IsRunning = false;
            }
        }
        
        ImGui::End();
    }
}

void ModelImporterPanel::StartImportProcess(const std::string& filepath)
{
    m_Importer.LoadModel(filepath);
    m_IsRunning = true;
}
