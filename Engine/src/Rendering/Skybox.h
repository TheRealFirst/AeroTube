#pragma once

#include <glm/glm.hpp>
#include "ShaderClass.h"

struct Skybox_Textures 
{
	char* right;
	char* left;
	char* top;
	char* bottom;
	char* front;
	char* back;
};

class Skybox
{
public:
	Skybox();
	void LoadSkybox(Skybox_Textures* textures);
	void DrawSkybox(glm::vec3 camPosition, glm::vec3 camOrientation, glm::vec3 camUp, uint32_t width, uint32_t height);
private:
	Shader skyboxShader;

	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int cubemapTexture;
};

