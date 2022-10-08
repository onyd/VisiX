#pragma once

#include <unordered_set>
#include <memory>
#include <glm/glm.hpp>

class Shader;
class Camera;
class BaseFX;

typedef std::shared_ptr<Shader> shader_ptr_t;

class Movable {
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

class Orientable {
	// TODO
};

class Scalable {
	// TODO
};

class Node {
	public:
		Node();

		Node(Node* parent);

		~Node();

		void addNode(Node* node);
		void removeNode(Node* node);

		virtual void draw(const Shader& shader, const Camera& camera) const;

	protected:

		inline Node* getParent() const {
			return m_parent;
		}

		inline void setParent(Node* parent) {
			m_parent = parent;
		}

		std::unordered_set<Node*> m_children;
		Node* m_parent;
};

class TransformNode : public Node, public Movable, public Orientable, public Scalable {
	public:
		virtual void draw(const Shader& shader, const Camera& camera) const;
};