#pragma once

#include <vector>

#include "Point.h"

class Polygon
{
public:
	Polygon() = default;
	Polygon(const Points2& points);

	Point2 support(const glm::vec2& direction) const;
	Point2 center() const;

private:
	Points2 m_points;
};