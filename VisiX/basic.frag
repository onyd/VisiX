#version 430 core

in vec3 wPos;
in vec3 wNormal;
in vec3 wColor;
in vec2 wTexCoords;

out vec4 FragColor;

void main()
{
	FragColor = lightColor;
};