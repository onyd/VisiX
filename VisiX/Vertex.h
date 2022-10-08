#pragma once

#include <glm/glm.hpp>


struct Vertex {
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec3 m_color;
	glm::vec2 m_tex_coords;
};