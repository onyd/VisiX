#include "ShaderGenerator.h"

#include "BaseEffect.h"

ShaderGenerator::ShaderGenerator(const MeshFormat& meshFormat)
{
	for (auto& [identifier, data] : meshFormat.getAttributes())
		vertexWriter.addVariable(identifier, data);
}

void ShaderGenerator::addFX(BasePositionEffect* fx)
{
	positionEffects.insert(fx);
}

void ShaderGenerator::removeFX(BasePositionEffect* fx)
{
	positionEffects.erase(fx);
}

void ShaderGenerator::addFX(BaseColorEffect* fx)
{
	colorEffects.insert(fx);
}

void ShaderGenerator::removeFX(BaseColorEffect* fx)
{
	colorEffects.erase(fx);
}

Shader ShaderGenerator::getShaderProgram()
{
	return Shader(vertexWriter.getShaderSource().c_str(), fragmentWriter.getShaderSource().c_str());
}