#pragma once
#include <optional>

#include "Rendering/Material.h"
#include "../../vendor/tinygltf/tiny_gltf.h"
#include "Rendering/Buffers/VertexBuffer.h"

namespace Engine
{
    struct ImporterMesh
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        std::string name;
        Ref<Material> material;
    };
    
    class ModelImporter
    {
    public:
        ModelImporter() = default;
        ~ModelImporter() = default;

        void LoadModel(const std::string& filepath);
        void SaveModel(const std::string& filepath, const std::optional<std::string>& name = std::nullopt);

    private:
        void ProcessNode(int nodeIndex, const glm::mat4& parentTransform, uint32_t& index);
        void ProcessPrimitive(const tinygltf::Primitive& primitive, const glm::mat4& transform, int materialIndex, uint32_t index);

        static glm::vec3 ReadVec3(const float* data);
        static glm::vec2 ReadVec2(const float* data);
    private:
        std::unique_ptr<tinygltf::Model> m_Model;
        std::vector<ImporterMesh> m_Meshes;

        std::string m_Path;
        std::string m_Name;

        std::vector<std::unordered_map<TextureType2D, Ref<Texture2D>>> m_MaterialTextures;
        std::unordered_map<std::string, Ref<Texture2D>> m_TextureCache;
        std::vector<Ref<Material>> m_Materials;
    };    
}

