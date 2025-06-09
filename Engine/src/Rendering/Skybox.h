#pragma once

#include <glm/glm.hpp>
#include "ShaderClass.h"

#include "Camera.h"

#include "Buffers\VertexArray.h"

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
	void DrawSkybox(Camera camera);
private:
	Shader skyboxShader;

	VertexArray VAO;
	unsigned int cubemapTexture;
};

