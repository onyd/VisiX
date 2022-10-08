#version 430 core

layout (location = 0) in vec4 pos;

uniform mat4 model;
uniform mat4 cameraMatrix;
uniform vec4 particleColor;

out vec4 wParticleColor;

void main()
{
	gl_PointSize = 10.0;
	gl_Position = cameraMatrix * model * pos;
	wParticleColor = particleColor;
}