#include "MeshFormat.h"

MeshFormat::MeshFormat(std::initializer_list<std::tuple<unsigned int, GLSLType, const char*>> attributes)
{
	for (auto& attr : attributes) {
		_attributes[std::get<2>(attr)] = std::make_shared<ShaderGlobalVariable>(GLSL_LAYOUT_IN, std::get<1>(attr), std::get<0>(attr));
	}
}

