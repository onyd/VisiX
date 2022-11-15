#pragma once

#include "Solid.h"
#include "GJK.h"

#define g 9.81

class DynamicSystemBase
{
public:
	virtual void step() = 0;
};

typedef std::shared_ptr<DynamicSystemBase> DynamicSystemPtr;

class SolidDynamicSystem : public DynamicSystemBase
{
public:
	SolidDynamicSystem(float h);

	virtual void step();
	void registerSolid(const SolidPtr& solid);

private:
	float m_h;
	std::vector<std::pair<SolidPtr, State>> m_solids_dstates;
};

class CollisionDynamicSystem : public DynamicSystemBase
{
public:
	virtual void step();
	void addCollisionRule(const Solid& solid1, const Solid& solid2);

private:
	std::vector<std::pair<const Solid&, const Solid&>> m_collision_rules;
	GJK m_collision_engine;
};