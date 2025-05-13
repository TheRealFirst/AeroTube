#pragma once

#include <unordered_map>
#include "Mesh.h"

#include "Texture.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <filesystem>

class Model
{
public:
    Model(const char* path)
    {
        std::filesystem::path modelPath(path);
        m_Path = modelPath.parent_path().string();
        loadModel(path);
    }
    void Draw(Shader& shader, const Camera& camera);
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    std::string m_Path;

    std::vector<Texture> loaded_textures;

    std::vector<glm::mat4> matricesMeshes;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};


