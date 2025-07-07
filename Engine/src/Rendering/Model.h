#pragma once

#include "../../vendor/tinygltf/tiny_gltf.h"
#include <unordered_map>
#include "Mesh.h"

#include "Texture.h"

#include <filesystem>


namespace Engine {
    class Model
    {
    public:
        Model(const std::string& path)
        {
            m_FullPath = path;
            std::filesystem::path modelPath(path);
            m_ParentPath = modelPath.parent_path().string();
            m_Name = modelPath.stem().string();
            LoadModel();
        }
        void Draw(const Camera& camera);
    private:
        void LoadModel();
        void ProcessNode(int nodeIndex, const glm::mat4& parentTransform);
        void ProcessPrimitive(const tinygltf::Primitive& primitive, const glm::mat4& transform, int materialIndex);

        static glm::vec3 ReadVec3(const float* data);
        static glm::vec2 ReadVec2(const float* data);

    private:
        std::vector<Mesh> m_Meshes;
        std::vector<Ref<Material>> m_Materials;

        std::string m_FullPath;
        std::string m_ParentPath;
        std::string m_Name;

        std::vector<glm::mat4> m_MatricesMeshes;

    };
}

