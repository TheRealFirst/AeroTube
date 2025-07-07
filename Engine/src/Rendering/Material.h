#pragma once
#include "Texture.h"

namespace Engine
{
    struct MaterialProbs
    {
        // Textures
        Ref<Texture2D> AlbedoTexture;    
        Ref<Texture2D> NormalTexture;    
        Ref<Texture2D> MetallicRoughnessTexture;    
        Ref<Texture2D> OcclusionTexture;
        Ref<Texture2D> EmissiveTexture;

        // Values
        glm::vec4 Albedo = glm::vec4(1.0f);
        float Metallic = 0;
        float Roughness = 0;
        float Occlusion = 0;
        float Emission = 0;

        // Shader
        Ref<Shader> Shader;
    };
    
    class Material
    {
    public:
        Material(const MaterialProbs& probs, const std::string& name);
        Material(const std::string& filepath);

        void SaveMaterial(std::string filepath);

        void Bind();
        void UnBind();

        std::string GetName() {return m_Name;}
        Ref<Shader> GetShader() {return m_MaterialProbs.Shader;}

        bool operator==(const Material& other) const;
    private:
        MaterialProbs m_MaterialProbs;
        std::string m_Name;
    };
}

