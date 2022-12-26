#pragma once

#include <glm/glm.hpp>

namespace vx 
{

class Movable 
{
public:
	inline void move(const glm::vec3& t) {
		m_pos += t;
	}

	inline void setPos(const glm::vec3& pos) {
		m_pos = pos;
	}
protected:
	glm::vec3 m_pos;
};

class Orientable 
{
	// TODO
};

class Scalable 
{
	// TODO
};

}