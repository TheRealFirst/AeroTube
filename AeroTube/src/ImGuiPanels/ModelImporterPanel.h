#pragma once

#include <Engine.h>

#include "Importers/ModelImporter.h"

class ModelImporterPanel
{
public:
    ModelImporterPanel() = default;
    ~ModelImporterPanel() = default;

    void OnImGuiRender();
    void StartImportProcess(const std::string& filepath);
private:
    Engine::ModelImporter m_Importer;
    Engine::Ref<Engine::Scene> m_Scene;
    bool m_IsRunning = false;

    std::string m_Name;
};
