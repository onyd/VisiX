#include "DynamicSystem.h"

SolidDynamicSystem::SolidDynamicSystem(float h) : m_h(h)
{
}

void SolidDynamicSystem::step()
{
	for (auto& [solid, dstate] : m_solids_dstates) {
		dstate.m_position += m_h * glm::vec2(0.0, -g);
		dstate.m_angle += m_h * 0.0;
		solid->translate(m_h * dstate.m_position);
		solid->rotate(m_h * dstate.m_angle);
		solid->update();
	}
}

void SolidDynamicSystem::registerSolid(const SolidPtr& solid)
{
	m_solids_dstates.emplace_back(std::make_pair(solid, State{ glm::vec2(0.0, 0.0), 0.0 }));
}
