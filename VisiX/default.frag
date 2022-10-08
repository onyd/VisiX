#version 430 core

in vec3 wPos;
in vec3 wNormal;
in vec3 wColor;
in vec2 wTexCoords;

out vec4 FragColor;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(wNormal);
	vec3 lightDirection = normalize(lightPos - wPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(cameraPos - wPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// outputs final color
	FragColor = (texture(diffuse0, wTexCoords) * (diffuse + ambient) + texture(specular0, wTexCoords).r * specular) * lightColor;
};