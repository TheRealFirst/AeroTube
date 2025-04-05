#version 450

out vec4 FragColor;

uniform vec4 lightColor;

void main()
{
	FragColor = lightColor;
}