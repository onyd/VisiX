#include <iostream>

#include "Node.h"
#include "ShaderClass.h"

Node::Node() : m_parent(NULL)
{
}

Node::Node(Node* parent) : m_parent(parent)
{
}

Node::~Node()
{
	for (auto child : m_children) {
		delete child;
	}
}

void Node::addNode(Node* node)
{
	if (!m_children.contains(node)) {
		m_children.insert(node);
		node->setParent(this);
	}
}

void Node::removeNode(Node* node)
{
	m_children.erase(node);
	node; setParent(NULL);
}

void Node::draw(const Shader& shader, const Camera& camera) const
{
	for (auto& child : m_children) {
		child->draw(shader, camera);
	}
}

void TransformNode::draw(const Shader& shader, const Camera& camera) const
{

}
