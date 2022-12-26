#include "Node.h"

namespace vx 
{

Node::Node()
{
}

void Node::draw(const Camera& camera, float dt) const
{
	for (auto& child : m_children) {
		child->draw(camera, dt);
	}
}

void Node::addNode(const NodePtr& node)
{
	if (!m_children.count(node) != 0) {
		m_children.insert(node);
	}
}

void Node::removeNode(const NodePtr& node)
{
	m_children.erase(node);
}

glm::mat4 TransformNode::getModelViewMatrix() const
{
	return glm::mat4(1.0);
}

}