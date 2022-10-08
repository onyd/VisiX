#pragma once

#include <unordered_set>
#include <memory>

#include "MeshFormat.h"

class BasePositionEffect;
class BaseColorEffect;

class ShaderGenerator {
private:
	ShaderWriter vertexWriter;
	ShaderWriter fragmentWriter;

	std::unordered_set<BasePositionEffect*> positionEffects;
	std::unordered_set<BaseColorEffect*> colorEffects;

public:
	ShaderGenerator(const MeshFormat& meshFormat);

	void addFX(BasePositionEffect* fx);

	void addFX(BaseColorEffect* fx);

	void removeFX(BasePositionEffect* fx);

	void removeFX(BaseColorEffect* fx);


	inline bool empty() {
		return positionEffects.empty() && colorEffects.empty();
	};

	Shader getShaderProgram();

};