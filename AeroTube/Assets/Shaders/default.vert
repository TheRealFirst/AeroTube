#version 450

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;

// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;

// Imports the camera matrix
uniform mat4 camMatrix;
// Imports the model matrix (includes translation, rotation, and scale)
uniform mat4 model;

void main()
{
    // Calculate current position using only the model matrix
    crntPos = vec3(model * vec4(aPos, 1.0f));
    
    // Transform the normal by the model matrix (excluding translation)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pass through color and texture coordinates
    color = aColor;
    texCoord = aTex;
    
    // Calculate final position
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}
