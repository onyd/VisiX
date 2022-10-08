#pragma once

#include <unordered_map>
#include <memory>
#include <utility>
#include <tuple>

#include "ShaderWriter.h"

class MeshFormat {
private:
	var_env_t _attributes;

public:
	MeshFormat(std::initializer_list<std::tuple<unsigned int, GLSLType, const char*>> attributes);

	inline const var_env_t& getAttributes() const {
		return _attributes;
	}

};