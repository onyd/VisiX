#include "GJK.h"

#include <stdexcept>

GJK::GJK()
{
}

bool GJK::intersects(const Polygon& shape1, const Polygon& shape2)
{
	m_simplex.clear();
	m_shape1 = shape1;
	m_shape2 = shape2;

	GJKStatus status = GJKStatus::RUNNING;
	while (status == GJKStatus::RUNNING)
		status = simplexStep();

	return status == GJKStatus::INTERSECTION;
}

bool GJK::addSupport()
{
	Point2 new_point = m_shape1.support(m_direction) - m_shape2.support(-m_direction);
	m_simplex.push_back(new_point);
	return glm::dot(m_direction, new_point) >= 0;
}

GJKStatus GJK::simplexStep()
{
	switch (m_simplex.size())
	{
	case 0:
		m_direction = m_shape2.center() - m_shape1.center();
		break;
	case 1:
		m_direction *= -1;
		break;
	case 2:
		glm::vec2 cb = m_simplex[1] - m_simplex[0];
		glm::vec2 c0 = -m_simplex[0];
		// Use the triple-cross-product to calculate a direction perpendicular to line cb in the direction of the origin
		m_direction = tripleProduct(cb, c0, cb);
		break;
	case 3:
		Point2 a = m_simplex[2];
		Point2 b = m_simplex[1];
		Point2 c = m_simplex[0];

		// Check if the current simplex contains the origin
		glm::vec2 a0 = -a;
		glm::vec2 ab = b - a;
		glm::vec2 ac = c - a;

		glm::vec2 ab_perpendicular = tripleProduct(ac, ab, ab);
		glm::vec2 ac_perpendicular = tripleProduct(ab, ac, ac);

		if (glm::dot(ab_perpendicular, a0) > 0) {
			// The origin is outside line ab, remove c and add a new support in the direction of ab_perpendicular
			m_simplex.erase(m_simplex.begin());
			m_direction = ab_perpendicular;
		}
		else if (glm::dot(ac_perpendicular, a0) > 0) {
			// The origin is outside line ac, remove b and add a new support in the direction of ac_perpendicular
			m_simplex.erase(m_simplex.end()-2);
			m_direction = ac_perpendicular;
		}
		else
			// The origin is inside both ab and ac so it must be inside the triangle thus we have intersection
			return GJKStatus::INTERSECTION;
		break;
	default:
		throw std::runtime_error("Simplex in 2D must have at most 3 points.");
	}

	return (addSupport()) ? GJKStatus::RUNNING : GJKStatus::NO_INTERSECTION;
}
