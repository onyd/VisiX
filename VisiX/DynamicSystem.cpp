#include "DynamicSystem.h"

SolidDynamicSystem::SolidDynamicSystem(float h) : m_h(h)
{
}

void SolidDynamicSystem::step()
{
	for (auto& [solid, dstate] : m_solids_dstates) {
		dstate.m_position += m_h * glm::vec2(0.0f, -g);
		dstate.m_angle += m_h * 0.0f;
		solid->translate(m_h * dstate.m_position);
		solid->rotate(m_h * dstate.m_angle);
		solid->update();
	}
}

void SolidDynamicSystem::registerSolid(const SolidPtr& solid)
{
	m_solids_dstates.emplace_back(std::make_pair(solid, State{ glm::vec2(0.0f, 0.0f), 0.0f }));
}

void CollisionDynamicSystem::step()
{
	for (const auto& [solid1, solid2] : m_collision_rules) {
		if (m_collision_engine.intersects(solid1.getPolygon(), solid2.getPolygon()))
			std::cout << "Collision" << std::endl;
	}
}

void CollisionDynamicSystem::addCollisionRule(const Solid& solid1, const Solid& solid2)
{
	m_collision_rules.push_back({ solid1, solid2 });
}
