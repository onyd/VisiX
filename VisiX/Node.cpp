#include "Node.h"

#include <iostream>

#include "ShaderClass.h"

Node::Node()
{
}

void Node::draw(const Camera& camera) const
{
	for (auto& child : m_children) {
		child->draw(camera);
	}
}

void Node::addNode(const NodePtr& node)
{
	if (!m_children.contains(node)) {
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

void RenderableNode::addRenderable(const RenderablePtr& renderable)
{
	m_renderables.emplace_back(renderable);
}

void RenderableNode::draw(const Camera& camera) const
{
	for (auto& renderable : m_renderables) {
		renderable->draw(camera);
	}
}