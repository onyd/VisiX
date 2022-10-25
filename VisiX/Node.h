#pragma once

#include <unordered_set>
#include <memory>
#include <glm/glm.hpp>

#include "Renderable.h"

class Shader;
class Camera;
class BaseFX;

typedef std::shared_ptr<Shader> ShaderPtr;

class Movable 
{
public:
	inline void move(const glm::vec3& t) {
		m_pos += t;
	}

	inline void setPos(const glm::vec3& pos) {
		m_pos = pos;
	}
protected:
	glm::vec3 m_pos;
};

class Orientable 
{
	// TODO
};

class Scalable 
{
	// TODO
};

class Node;
typedef std::shared_ptr<Node> NodePtr;

class Node 
{
public:
	Node();

	Node(const NodePtr& parent);

	void addNode(const NodePtr& node);
	void removeNode(const NodePtr& node);

	virtual void draw(const Camera& camera) const;

protected:
	std::unordered_set<NodePtr> m_children;
};

class TransformNode : public Node, public Movable, public Orientable, public Scalable 
{
public:
	glm::mat4 getModelViewMatrix() const;
};

class RenderableNode : public Node
{
public:
	RenderableNode() = default;

	void addRenderable(const RenderablePtr& renderable);

	virtual void draw(const Camera& camera) const;

protected:
	std::vector<std::shared_ptr<Renderable>> m_renderables;
};