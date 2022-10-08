#pragma once

#include "Node.h"

class ShaderWriter;

class BasePositionEffect {
	public:
		virtual void codeGen(ShaderWriter& vertexWriter) const;

	protected:
		BasePositionEffect() = default;
};

class BaseColorEffect {
public:
	virtual void codeGen(ShaderWriter& vertexWriter) const;

protected:
	BaseColorEffect() = default;
};