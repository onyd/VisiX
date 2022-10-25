#include "DynamicSystem.h"

SolidDynamicSystem::SolidDynamicSystem(float h) : m_h(h)
{
}

void SolidDynamicSystem::step()
{
	for (auto& [solid, dstate] : m_solids_dstates) {
		dstate += m_h * glm::vec3(0.0, -g, 0.0);
		solid->transform(m_h * dstate);
		solid->update();
	}
}

void SolidDynamicSystem::registerSolid(const SolidPtr& solid)
{
	m_solids_dstates.emplace_back(std::make_pair(solid, glm::vec3(0.0, 0.0, 0.0)));
}
