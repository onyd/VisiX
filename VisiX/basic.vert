#version 430 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;
layout (location = 3) in vec2 vTexCoords;

out vec3 wPos;
out vec3 wNormal;
out vec3 wColor; 
out vec2 wTexCoords;

uniform mat4 cameraMatrix;
uniform mat4 model;

void main()
{
	wPos = vec3(model * vec4(vPos, 1.0f));
	gl_Position = cameraMatrix * vec4(wPos, 1.0f);
	wColor = vColor;
	wTexCoords = vTexCoords;
	wNormal = vNormal;
};