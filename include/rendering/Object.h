#pragma once

#include "Node.h"
#include "Renderable.h"

namespace vx
{

class Object : public Node
{
public:
    Object(const RenderablePtr& renderable);

	virtual void draw(const Camera& camera, float dt) const;

private:
    RenderablePtr m_renderable;
};

} // namespace vx
