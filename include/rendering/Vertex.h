#pragma once

#include <glm/glm.hpp>

namespace vx 
{

struct Vertex {
	glm::vec3 m_position;
	glm::vec3 normal;
	glm::vec3 m_color;
	glm::vec2 m_tex_coords;
};

}