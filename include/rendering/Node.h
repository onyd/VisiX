#pragma once

#include <unordered_set>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include "Renderable.h"
#include "ShaderClass.h"
#include "Camera.h"

namespace vx 
{

class Node;
typedef std::shared_ptr<Node> NodePtr;

class Node 
{
public:
	Node();

	Node(const NodePtr& parent);

	void addNode(const NodePtr& node);
	void removeNode(const NodePtr& node);

	virtual void draw(const Camera& camera, float dt) const;

protected:
	std::unordered_set<NodePtr> m_children;
};

class TransformNode : public Node, public Movable, public Orientable, public Scalable 
{
public:
	glm::mat4 getModelViewMatrix() const;
};

}