#pragma once

#include "../../vendor/tinygltf/tiny_gltf.h"
#include <unordered_map>
#include "Mesh.h"

#include "Texture.h"

#include <filesystem>


// namespace tinygltf {
//     class Model;
//     struct Primitive;
// }

class Model
{
public:
    Model(std::string path)
    {
        std::filesystem::path modelPath(path);
        m_Path = modelPath.parent_path().string();
        loadModel(path);
    }
    void Draw(Shader& shader, const Camera& camera);
private:
    void loadModel(std::string &path);
    void ProcessNode(int nodeIndex, const glm::mat4& parentTransform);
    void ProcessPrimitive(const tinygltf::Primitive& primitive, const glm::mat4& transform);

    static glm::vec3 ReadVec3(const float* data);
    static glm::vec2 ReadVec2(const float* data);

private:
    // model data
    std::unique_ptr<tinygltf::Model> m_Model;
    std::vector<Mesh> m_Meshes;
    std::string directory;

    std::string m_Path;

    std::vector<std::unordered_map<TextureType, Engine::Ref<Texture>>> material_textures;
    std::vector<Engine::Ref<Texture>> loaded_textures;

    std::vector<glm::mat4> matricesMeshes;

};


