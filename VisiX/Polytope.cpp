#include "Polytope.h"

Polygon::Polygon(const Points2& points) : m_points(points)
{
}

Point2 Polygon::support(const glm::vec2& direction) const
{
	float furthest_distance = std::numeric_limits<float>::lowest();
	auto it = m_points.begin();
	auto furthest_point = it++;

	while (it != m_points.end()) {
		float d = glm::dot(*it, direction);
		if (d > furthest_distance) {
			furthest_distance = d;
			furthest_point = it;
		}
		it++;
	}

	return *furthest_point;
}

Point2 Polygon::center() const
{
	Point2 center = glm::vec2(0.0f, 0.0f);
	for (const auto& p : m_points) {
		center += p;
	}
	return center / (float)m_points.size();
}
