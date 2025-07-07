#pragma once

#include "glm/glm.hpp"
#include "yaml-cpp/yaml.h"

#include "Rendering/Buffers/VertexBuffer.h"

namespace YAML {

    template<>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };
    
    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Engine::Vertex>
    {
        static Node encode(const Engine::Vertex& v)
        {
            Node node;
            node["Position"] = v.position;
            node["Normal"]   = v.normal;
            node["Color"]    = v.color;
            node["TexUV"]    = v.texUV;
            return node;
        }

        static bool decode(const Node& node, Engine::Vertex& v)
        {
            if (!node["Position"] || !node["Normal"] || !node["Color"] || !node["TexUV"])
                return false;

            v.position = node["Position"].as<glm::vec3>();
            v.normal   = node["Normal"].as<glm::vec3>();
            v.color    = node["Color"].as<glm::vec3>();
            v.texUV    = node["TexUV"].as<glm::vec2>();
            return true;
        }
    };
}

namespace Engine
{
    inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }
    
    inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    inline YAML::Emitter& operator<<(YAML::Emitter& out, const Engine::Vertex& v)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Position" << YAML::Value << v.position;
        out << YAML::Key << "Normal"   << YAML::Value << v.normal;
        out << YAML::Key << "Color"    << YAML::Value << v.color;
        out << YAML::Key << "TexUV"    << YAML::Value << v.texUV;
        out << YAML::EndMap;
        return out;
    }
}
