#version 430 core

out vec4 FragColor;

in vec4 wParticleColor;

void main()
{
	FragColor = wParticleColor;
}