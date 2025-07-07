#include "atpch.h"
#include "Material.h"

#include "yaml-cpp/yaml.h"
#include "Utils/YamlHelpers.h"

namespace Engine
{
    Material::Material(const MaterialProbs& probs, const std::string& name) : m_MaterialProbs(probs), m_Name(name)
    {
        LOG_DEBUG(m_Name.c_str())
        m_MaterialProbs.Shader->Bind();
        m_MaterialProbs.Shader->SetFloat4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    Material::Material(const std::string& filepath)
    {
        YAML::Node data = YAML::LoadFile(filepath);

        m_Name = data["Name"].as<std::string>("UnnamedMaterial");

        std::string shaderName = data["Shader"].as<std::string>("None");
        if (shaderName != "None")
        {
            m_MaterialProbs.Shader = Shader::Create("Assets/Shaders/" + shaderName); 
            m_MaterialProbs.Shader->Bind();
            m_MaterialProbs.Shader->SetFloat4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // TODO: Light Management
        }
        else
        {
            LOG_WARN("Material %s has no Shader", m_Name.c_str())
            m_MaterialProbs.Shader = Shader::Create("Assets/Shaders/default.glsl"); 
            m_MaterialProbs.Shader->Bind();
            m_MaterialProbs.Shader->SetFloat4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // TODO: Light Management
        }

        YAML::Node textures = data["Textures"];
        if (textures)
        {
            auto loadTex = [](const YAML::Node& node, TextureType2D type) -> Ref<Texture2D> // TODO: Load Relative Path
            {
                std::string path = node.as<std::string>("");
                return path.empty() ? nullptr : Texture2D::Create(path, type);
            };

            m_MaterialProbs.AlbedoTexture            = loadTex(textures["Albedo"], TextureType2D::Diffuse);
            m_MaterialProbs.NormalTexture            = loadTex(textures["Normal"], TextureType2D::Normal);
            m_MaterialProbs.MetallicRoughnessTexture = loadTex(textures["MetallicRoughness"], TextureType2D::MetallicRoughness);
            m_MaterialProbs.OcclusionTexture         = loadTex(textures["Occlusion"], TextureType2D::Occlusion);
            m_MaterialProbs.EmissiveTexture          = loadTex(textures["Emissive"], TextureType2D::Emissive);
        }

        YAML::Node values = data["Values"];
        if (values)
        {
            m_MaterialProbs.Albedo    = values["Albedo"].as<glm::vec4>(glm::vec4(1.0f));
            m_MaterialProbs.Metallic  = values["Metallic"].as<float>(0.0f);
            m_MaterialProbs.Roughness = values["Roughness"].as<float>(0.0f);
            m_MaterialProbs.Occlusion = values["Occlusion"].as<float>(0.0f);
            m_MaterialProbs.Emission  = values["Emission"].as<float>(0.0f);
        }
    }

    void Material::SaveMaterial(std::string filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;

        out << YAML::Key << "Name" << YAML::Value << m_Name;
        out << YAML::Key << "Shader" << YAML::Value << (m_MaterialProbs.Shader ? m_MaterialProbs.Shader->GetName() + ".glsl" : "None");

        out << YAML::Key << "Textures" << YAML::Value << YAML::BeginMap;

        std::string texturePath = filepath + "/Textures";

        auto moveAndNormalize = [&](Ref<Texture2D>& tex) -> std::string //TODO: Only store relative path
        {
            if (!tex) return "";
            tex->MoveLocation(texturePath);
            std::string path = tex->GetPath();
            std::replace(path.begin(), path.end(), '\\', '/');
            return path;
        };

        out << YAML::Key << "Albedo"            << YAML::Value << moveAndNormalize(m_MaterialProbs.AlbedoTexture);
        out << YAML::Key << "Normal"            << YAML::Value << moveAndNormalize(m_MaterialProbs.NormalTexture);
        out << YAML::Key << "MetallicRoughness" << YAML::Value << moveAndNormalize(m_MaterialProbs.MetallicRoughnessTexture);
        out << YAML::Key << "Occlusion"         << YAML::Value << moveAndNormalize(m_MaterialProbs.OcclusionTexture);
        out << YAML::Key << "Emissive"          << YAML::Value << moveAndNormalize(m_MaterialProbs.EmissiveTexture);
        out << YAML::EndMap;

        out << YAML::Key << "Values" << YAML::Value << YAML::BeginMap;
        out << YAML::Key << "Albedo"    << YAML::Value << m_MaterialProbs.Albedo;
        out << YAML::Key << "Metallic"  << YAML::Value << m_MaterialProbs.Metallic;
        out << YAML::Key << "Roughness" << YAML::Value << m_MaterialProbs.Roughness;
        out << YAML::Key << "Occlusion" << YAML::Value << m_MaterialProbs.Occlusion;
        out << YAML::Key << "Emission"  << YAML::Value << m_MaterialProbs.Emission;
        out << YAML::EndMap;

        out << YAML::EndMap;

        std::string materialPath = filepath + "/" + m_Name + ".atmat";
        std::replace(materialPath.begin(), materialPath.end(), '\\', '/');

        std::ofstream fout(materialPath);
        if (!fout.is_open())
        {
            LOG_ERROR("Failed to open material file for writing: %s", materialPath.c_str());
            return;
        }
        fout << out.c_str();
    }


    void Material::Bind()
    {
        m_MaterialProbs.Shader->Bind();

        if(m_MaterialProbs.AlbedoTexture)
        {
            m_MaterialProbs.Shader->SetInt(m_MaterialProbs.AlbedoTexture->GetTypeAsUniform(), 0);
            m_MaterialProbs.AlbedoTexture->Bind(0);
        }
        else
        {
            m_MaterialProbs.Shader->SetFloat4("Albedo", m_MaterialProbs.Albedo);
        }
        
        if(m_MaterialProbs.NormalTexture)
        {
            m_MaterialProbs.Shader->SetInt(m_MaterialProbs.NormalTexture->GetTypeAsUniform(), 1);
            m_MaterialProbs.NormalTexture->Bind(1);
        }
        
        if(m_MaterialProbs.MetallicRoughnessTexture)
        {
            m_MaterialProbs.Shader->SetInt(m_MaterialProbs.MetallicRoughnessTexture->GetTypeAsUniform(), 2);
            m_MaterialProbs.MetallicRoughnessTexture->Bind(2);
        }
        else
        {
            m_MaterialProbs.Shader->SetFloat("Metallic", m_MaterialProbs.Metallic);
            m_MaterialProbs.Shader->SetFloat("Roughness", m_MaterialProbs.Roughness);
        }
        
        if(m_MaterialProbs.OcclusionTexture)
        {
            m_MaterialProbs.Shader->SetInt(m_MaterialProbs.OcclusionTexture->GetTypeAsUniform(), 3);
            m_MaterialProbs.OcclusionTexture->Bind(3);
        }
        else
        {
            m_MaterialProbs.Shader->SetFloat("Occlusion", m_MaterialProbs.Occlusion);
        }
        
        if(m_MaterialProbs.EmissiveTexture)
        {
            m_MaterialProbs.Shader->SetInt(m_MaterialProbs.EmissiveTexture->GetTypeAsUniform(), 4);
            m_MaterialProbs.EmissiveTexture->Bind(4);
        }
        else
        {
            m_MaterialProbs.Shader->SetFloat("Emission", m_MaterialProbs.Emission);
        }
    }

    void Material::UnBind()
    {
        m_MaterialProbs.Shader->UnBind();
    }

    bool Material::operator==(const Material& other) const
    {
        return m_Name == other.m_Name;
    }
}

