#pragma once

#include <string>

#include "Buffers/VertexArray.h"
#include "Camera.h"
#include "Texture.h"

namespace Engine {
    class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::unordered_map<TextureType2D, Ref<Texture2D>>& textures);
        // Draws the mesh
        void Draw
        (
            Shader& shader,
            glm::mat4 matrix = glm::mat4(1.0f),
            glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
        );
        void Delete();

    private:
        // mesh data
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::unordered_map<TextureType2D, Ref<Texture2D>> m_Textures;


        // render data
        Ref<VertexArray> m_VertexArray;
    };
}
