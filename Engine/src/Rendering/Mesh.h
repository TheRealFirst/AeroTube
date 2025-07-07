#pragma once

#include <string>

#include "Buffers/VertexArray.h"
#include "Camera.h"
#include "Material.h"
#include "Texture.h"

namespace Engine {
    struct MeshAttributes
    {
        std::string Name;
        
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        Ref<Material> Material;
    };
    
    class Mesh {
    public:
        Mesh(const MeshAttributes& attributes);
        // Draws the mesh
        void Draw
        (
            glm::mat4 matrix = glm::mat4(1.0f),
            glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
        );
        void Delete();

        
        Ref<Material> GetMaterial() const { return m_Attributes.Material; }
    private:
        // mesh data
        MeshAttributes m_Attributes;


        // render data
        Ref<VertexArray> m_VertexArray;
    };
}
