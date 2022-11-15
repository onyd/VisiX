#pragma once

#include "Polytope.h"

enum class GJKStatus
{
	RUNNING = 0,
	INTERSECTION,
	NO_INTERSECTION
};

class GJK
{
public:
	GJK();
	
	bool intersects(const Polygon& shape1, const Polygon& shape2);

private:
	bool addSupport();
	GJKStatus simplexStep();

	Points2 m_simplex;
	glm::vec2 m_direction;
	Polygon m_shape1, m_shape2;
};