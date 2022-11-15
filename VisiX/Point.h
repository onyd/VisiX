#pragma once

#include <glm/glm.hpp>
#include <vector>

typedef glm::vec2 Point2; 
typedef glm::vec3 Point3;

typedef std::vector<Point2> Points2;
typedef std::vector<Point3> Points3;

inline glm::vec2 tripleProduct(const Point2& a, const Point2& b, const Point2& c)
{
	float ac = glm::dot(a, c);
	float bc = glm::dot(b, c);

	return b * ac - a * bc;
}