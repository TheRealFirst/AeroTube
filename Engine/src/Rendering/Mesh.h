#pragma once

#include <string>

#include "Buffers/VertexArray.h"
#include "Camera.h"
#include "Texture.h"

namespace Engine {
    class Mesh {
    public:
        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::unordered_map<TextureType2D, Ref<Texture2D>>& textures);
        // Draws the mesh
        void Draw
        (
            Shader& shader,
            const Camera& camera,
            glm::mat4 matrix = glm::mat4(1.0f),
            glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
        );
        void Delete();

    private:
        // mesh data
        std::vector<Vertex> m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::unordered_map<TextureType2D, Ref<Texture2D>> m_Textures;


        // render data
        VertexArray m_VertexArray;
    };
}
