#include "Object.h"

namespace vx
{
    
Object::Object(const RenderablePtr& renderable)
    : m_renderable(renderable)
{}

void Object::draw(const Camera& camera, float dt) const
{
    m_renderable->draw(camera, dt);
}

} // namespace vx
