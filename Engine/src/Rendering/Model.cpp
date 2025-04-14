#include "atpch.h"
#include"Model.h"

// Initialize static model cache
std::unordered_map<std::string, Model*> Model::s_ModelCache;

Model::Model(const char* file)
{
	// Make a JSON object
	std::string text = get_file_contents(file);
	JSON = json::parse(text);

	// Get the binary data
	Model::file = file;
	data = getData();

	// Traverse all nodes
	traverseNode(0);
}

Model* Model::LoadModel(const char* file)
{
	// Check if the model is already in the cache
	std::string filePath(file);
	auto it = s_ModelCache.find(filePath);

	if (it != s_ModelCache.end())
	{
		// Model found in cache, return it
		LOG_INFO("Model loaded from cache: {0}", filePath);
		return it->second;
	}

	// Model not in cache, load it
	LOG_INFO("Loading model: {0}", filePath);
	Model* model = new Model(file);

	// Add to cache
	s_ModelCache[filePath] = model;

	return model;
}

void Model::ClearModelCache()
{
	// Delete all models in the cache
	for (auto& pair : s_ModelCache)
	{
		delete pair.second;
	}

	// Clear the cache
	s_ModelCache.clear();
	LOG_INFO("Model cache cleared");
}

void Model::Draw(Shader& shader, const Camera& camera)
{
	// Go over all meshes and draw each one
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i]);
	}
}

void Model::loadMesh(unsigned int indMesh)
{
	try
	{
		// Check if the mesh index is valid
		if (indMesh >= JSON["meshes"].size())
		{
			LOG_ERROR("Invalid mesh index: {0}", indMesh);
			return;
		}

		// Check if the mesh has primitives
		if (!JSON["meshes"][indMesh].contains("primitives") || JSON["meshes"][indMesh]["primitives"].size() == 0)
		{
			LOG_ERROR("Mesh {0} has no primitives", indMesh);
			return;
		}

		// Check if the primitive has attributes
		if (!JSON["meshes"][indMesh]["primitives"][0].contains("attributes"))
		{
			LOG_ERROR("Mesh {0} primitive 0 has no attributes", indMesh);
			return;
		}

		// Get all accessor indices with error checking
		if (!JSON["meshes"][indMesh]["primitives"][0]["attributes"].contains("POSITION"))
		{
			LOG_ERROR("Mesh {0} primitive 0 has no POSITION attribute", indMesh);
			return;
		}
		unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];

		// Check if the normal attribute exists
		unsigned int normalAccInd = 0;
		bool hasNormals = JSON["meshes"][indMesh]["primitives"][0]["attributes"].contains("NORMAL");
		if (hasNormals)
		{
			normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
		}

		// Check if the texture coordinate attribute exists
		unsigned int texAccInd = 0;
		bool hasTexCoords = JSON["meshes"][indMesh]["primitives"][0]["attributes"].contains("TEXCOORD_0");
		if (hasTexCoords)
		{
			texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
		}

		// Check if the indices attribute exists
		if (!JSON["meshes"][indMesh]["primitives"][0].contains("indices"))
		{
			LOG_ERROR("Mesh {0} primitive 0 has no indices", indMesh);
			return;
		}
		unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

		// Use accessor indices to get all vertices components
		std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
		std::vector<glm::vec3> positions = groupFloatsVec3(posVec);

		// Get normals if they exist
		std::vector<glm::vec3> normals;
		if (hasNormals)
		{
			std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
			normals = groupFloatsVec3(normalVec);
		}
		else
		{
			// If normals don't exist, create default normals
			normals = std::vector<glm::vec3>(positions.size(), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		// Get texture coordinates if they exist
		std::vector<glm::vec2> texUVs;
		if (hasTexCoords)
		{
			std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
			texUVs = groupFloatsVec2(texVec);
		}
		else
		{
			// If texture coordinates don't exist, create default UVs
			texUVs = std::vector<glm::vec2>(positions.size(), glm::vec2(0.0f, 0.0f));
		}

		// Combine all the vertex components and also get the indices and textures
		std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
		std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
		std::vector<Texture> textures = getTextures();

		// Combine the vertices, indices, and textures into a mesh
		meshes.push_back(Mesh(vertices, indices, textures));
	}
	catch (const std::exception& e)
	{
		LOG_ERROR("Error loading mesh {0}: {1}", indMesh, e.what());
	}
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	// Current node
	json node = JSON["nodes"][nextNode];

	// Get translation if it exists
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);
		translation = glm::make_vec3(transValues);
	}
	// Get quaternion if it exists
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2],
			node["rotation"][3],
		};
		rotation = glm::make_quat(rotValues);
	}
	// Get scale if it exists
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}
	// Get matrix if it exists
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = glm::make_mat4(matValues);
	}

	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Multiply all matrices together
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	// Check if the node contains a mesh and if it does load it
	if (node.find("mesh") != node.end())
	{
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	// Check if the node has children, and if it does, apply this function to them with the matNextNode
	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			traverseNode(node["children"][i], matNextNode);
	}
}

std::vector<unsigned char> Model::getData()
{
	// Create a place to store the raw text, and get the uri of the .bin file
	std::string bytesText;
	std::string uri = JSON["buffers"][0]["uri"];

	// Store raw text data into bytesText
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = get_file_contents((fileDirectory + uri).c_str());

	// Transform the raw text data into bytes and put them in a vector
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float> Model::getFloats(json accessor)
{
	// Get properties from the accessor
	// Check if bufferView exists in the accessor
	if (!accessor.contains("bufferView"))
	{
		// If bufferView is not present, return an empty vector
		LOG_WARN("Accessor does not contain bufferView");
		return std::vector<float>();
	}

	unsigned int buffViewInd = accessor["bufferView"];
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	// Get properties from the bufferView
	// Check if the bufferView index is valid
	if (buffViewInd >= JSON["bufferViews"].size())
	{
		LOG_ERROR("Invalid bufferView index: {0}", buffViewInd);
		return std::vector<float>();
	}

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Pre-allocate the vector to avoid reallocations
	std::vector<float> floatVec;
	// Calculate the expected size based on the type
	unsigned int numComponents = 1;
	if (type == "SCALAR") numComponents = 1;
	else if (type == "VEC2") numComponents = 2;
	else if (type == "VEC3") numComponents = 3;
	else if (type == "VEC4") numComponents = 4;
	else if (type == "MAT3") numComponents = 9;
	else if (type == "MAT4") numComponents = 16;

	// Reserve space for all elements
	floatVec.reserve(count * numComponents);

	// Interpret the type and store it into numPerVert
	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

	// Go over all the bytes in the data at the correct place using the properties from above
	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i += 4)
	{
		unsigned char bytes[] = { data[i], data[i + 1], data[i + 2], data[i + 3] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
	// Get properties from the accessor
	// Check if bufferView exists in the accessor
	if (!accessor.contains("bufferView"))
	{
		// If bufferView is not present, return an empty vector
		LOG_WARN("Accessor does not contain bufferView");
		return std::vector<GLuint>();
	}

	unsigned int buffViewInd = accessor["bufferView"];
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	// Get properties from the bufferView
	// Check if the bufferView index is valid
	if (buffViewInd >= JSON["bufferViews"].size())
	{
		LOG_ERROR("Invalid bufferView index: {0}", buffViewInd);
		return std::vector<GLuint>();
	}

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Pre-allocate the vector to avoid reallocations
	std::vector<GLuint> indices;
	indices.reserve(count);

	// Get indices with regards to their type: unsigned int, unsigned short, or short
	unsigned int beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i += 4)
		{
			unsigned char bytes[] = { data[i], data[i + 1], data[i + 2], data[i + 3] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i += 2)
		{
			unsigned char bytes[] = { data[i], data[i + 1] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i += 2)
		{
			unsigned char bytes[] = { data[i], data[i + 1] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

std::vector<Texture> Model::getTextures()
{
	std::vector<Texture> textures;

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// Go over all images
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		// uri of current texture
		std::string texPath = JSON["images"][i]["uri"];

		// Check if the texture has already been loaded
		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if (loadedTexName[j] == texPath)
			{
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}

		// If the texture has been loaded, skip this
		if (!skip)
		{
			// Log the texture path for debugging
			LOG_INFO("Processing texture: {0}", texPath);

			// Create the full path to the texture
			std::string fullPath = fileDirectory + texPath;

			// Check if the file exists
			FILE* file = fopen(fullPath.c_str(), "r");
			if (!file)
			{
				// File doesn't exist, log an error
				LOG_ERROR("Texture file not found: {0}", fullPath);

				// Try to find the texture in a different location
				// For adamHead model, the textures are in a different location
				size_t lastSlash = texPath.find_last_of("/\\");
				if (lastSlash != std::string::npos)
				{
					std::string textureName = texPath.substr(lastSlash + 1);
					std::string alternativePath = fileDirectory + textureName;

					LOG_INFO("Trying alternative path: {0}", alternativePath);

					file = fopen(alternativePath.c_str(), "r");
					if (file)
					{
						fclose(file);
						fullPath = alternativePath;
					}
					else
					{
						LOG_ERROR("Alternative texture file not found: {0}", alternativePath);
						return textures; // Skip this texture
					}
				}
				else
				{
					return textures; // Skip this texture
				}
			}
			else
			{
				fclose(file);
			}

			// Determine the texture type based on the filename
			if (texPath.find("baseColor") != std::string::npos || texPath.find("_a.") != std::string::npos)
			{
				// Load diffuse texture
				LOG_INFO("Loading diffuse texture: {0}", fullPath);
				Texture diffuse = Texture::LoadTexture(fullPath.c_str(), "diffuse", loadedTex.size());
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}
			else if (texPath.find("metallicRoughness") != std::string::npos || texPath.find("_sg.") != std::string::npos)
			{
				// Load specular texture
				LOG_INFO("Loading specular texture: {0}", fullPath);
				Texture specular = Texture::LoadTexture(fullPath.c_str(), "specular", loadedTex.size());
				textures.push_back(specular);
				loadedTex.push_back(specular);
				loadedTexName.push_back(texPath);
			}
			else if (texPath.find("_n.") != std::string::npos)
			{
				// Load normal map
				LOG_INFO("Loading normal map: {0}", fullPath);
				Texture normal = Texture::LoadTexture(fullPath.c_str(), "normal", loadedTex.size());
				textures.push_back(normal);
				loadedTex.push_back(normal);
				loadedTexName.push_back(texPath);
			}
			else if (texPath.find("_o.") != std::string::npos)
			{
				// Load occlusion map
				LOG_INFO("Loading occlusion map: {0}", fullPath);
				Texture occlusion = Texture::LoadTexture(fullPath.c_str(), "specular", loadedTex.size());
				textures.push_back(occlusion);
				loadedTex.push_back(occlusion);
				loadedTexName.push_back(texPath);
			}
			else
			{
				// Unknown texture type, load as diffuse
				LOG_INFO("Loading unknown texture type as diffuse: {0}", fullPath);
				Texture diffuse = Texture::LoadTexture(fullPath.c_str(), "diffuse", loadedTex.size());
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}
		}
	}

	return textures;
}

std::vector<Vertex> Model::assembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	// Pre-allocate the vertices vector to avoid reallocations
	std::vector<Vertex> vertices;
	vertices.reserve(positions.size());

	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				texUVs[i]
			}
		);
	}
	return vertices;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
	const unsigned int floatsPerVector = 2;

	// Pre-allocate the vectors to avoid reallocations
	std::vector<glm::vec2> vectors;
	vectors.reserve(floatVec.size() / floatsPerVector);

	for (unsigned int i = 0; i < floatVec.size(); i += floatsPerVector)
	{
		vectors.push_back(glm::vec2(0, 0));

		for (unsigned int j = 0; j < floatsPerVector; j++)
		{
			vectors.back()[j] = floatVec[i + j];
		}
	}
	return vectors;
}
std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
	const unsigned int floatsPerVector = 3;

	// Pre-allocate the vectors to avoid reallocations
	std::vector<glm::vec3> vectors;
	vectors.reserve(floatVec.size() / floatsPerVector);

	for (unsigned int i = 0; i < floatVec.size(); i += floatsPerVector)
	{
		vectors.push_back(glm::vec3(0, 0, 0));

		for (unsigned int j = 0; j < floatsPerVector; j++)
		{
			vectors.back()[j] = floatVec[i + j];
		}
	}
	return vectors;
}
std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
	const unsigned int floatsPerVector = 4;

	// Pre-allocate the vectors to avoid reallocations
	std::vector<glm::vec4> vectors;
	vectors.reserve(floatVec.size() / floatsPerVector);

	for (unsigned int i = 0; i < floatVec.size(); i += floatsPerVector)
	{
		vectors.push_back(glm::vec4(0, 0, 0, 0));

		for (unsigned int j = 0; j < floatsPerVector; j++)
		{
			vectors.back()[j] = floatVec[i + j];
		}
	}
	return vectors;
}