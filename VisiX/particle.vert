#version 430 core

layout (location = 0) in vec4 pos;

uniform mat4 model;
uniform mat4 cameraMatrix;

out vec4 wParticleColor;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	gl_PointSize = 3.0;
	gl_Position = cameraMatrix * model * pos;
	wParticleColor = vec4(hsv2rgb(vec3(length(pos.xyz), 0.5, 0.5)), 0.5);
}