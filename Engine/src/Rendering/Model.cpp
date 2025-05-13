#include "atpch.h"
#include"Model.h"
#include <glm\glm.hpp>
#include "glad\glad.h"


void Model::Draw(Shader& shader, const Camera& camera)
{
	// Verify shader is active
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	if (currentProgram != shader.ID) {
		LOG_ERROR("Shader program mismatch! Expected: %d, Got: %d", shader.ID, currentProgram);
		shader.Activate();
	}

	LOG_DEBUG("Drawing model with %d meshes", meshes.size());
	
	// Check if we have any meshes to draw
	if (meshes.empty()) {
		LOG_WARN("Model has no meshes to draw!");
		return;
	}

	for (unsigned int i = 0; i < meshes.size(); i++) {
		LOG_DEBUG("Drawing mesh %d with %d vertices", i, meshes[i].vertices.size());
		
		// Verify mesh data
		if (meshes[i].vertices.empty() || meshes[i].indices.empty()) {
			LOG_ERROR("Mesh %d has invalid data! Vertices: %d, Indices: %d", 
				i, meshes[i].vertices.size(), meshes[i].indices.size());
			continue;
		}
		
		meshes[i].Draw(shader, camera);
		// meshes[i].Draw(shader, camera, matricesMeshes[i]);
		
		// Check for errors after each mesh
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			LOG_ERROR("OpenGL error after drawing mesh %d: 0x%x", i, err);
		}
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
		aiProcess_JoinIdenticalVertices |
		aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("ASSIMP ERROR: %s", importer.GetErrorString());
		return;
	}

	directory = path.substr(0, path.find_last_of("/"));
	LOG_INFO("Loading model from path: %s", path.c_str());
	LOG_INFO("Model directory: %s", directory.c_str());
	LOG_INFO("Number of materials: %d", scene->mNumMaterials);
	LOG_INFO("Number of meshes: %d", scene->mNumMeshes);

	processNode(scene->mRootNode, scene);

	LOG_INFO("Model loaded successfully. Total meshes: %d, Total textures: %d", 
		meshes.size(), loaded_textures.size());
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// Position
		vertex.position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		// Normals
		if (mesh->HasNormals())
		{
			vertex.normal = glm::vec3(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);
		}

		// Texture coordinates
		if (mesh->mTextureCoords[0])
		{
			vertex.texUV = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else
		{
			vertex.texUV = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// Process indices - Make sure we're only processing triangles
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	LOG_DEBUG("Processed mesh - Vertices: %d, Indices: %d, Textures: %d",
		vertices.size(), indices.size(), textures.size());

	if (indices.empty()) {
		LOG_ERROR("No valid indices generated for mesh");
		// Create default indices for triangles if none exist
		indices.reserve(vertices.size());
		for (unsigned int i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
		}
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		std::string fullpath = std::string(m_Path) + "/" + std::string(str.C_Str());
		
		// Compare strings using std::string comparison instead of strcmp
		for (unsigned int j = 0; j < loaded_textures.size(); j++)
		{
			if (loaded_textures[j].GetPath() == str.C_Str())
			{
				textures.push_back(loaded_textures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture(fullpath.c_str(), typeName.c_str(), 0);
			textures.push_back(texture);
			loaded_textures.push_back(texture);
			LOG_DEBUG("Loaded Texture");
		}
	}
	return textures;
}
