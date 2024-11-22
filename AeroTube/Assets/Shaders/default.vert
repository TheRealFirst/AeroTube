#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;

layout (location = 2) in vec2 aTex;

layout (location = 3) in vec3 aNormal;

out vec2 texCoord;

// Outputs the color for the Fragment Shader
out vec3 color;

out vec3 Normal;
out vec3 crntPos;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	crntPos = vec3(model * vec4(aPos, 1.0f));
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.0);
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	texCoord = aTex;
	Normal = aNormal;
}