#pragma once

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Renderable.h"

class Viewer;

class Solid
{
public:
	Solid(const glm::vec2& position, float angle);

	glm::mat4 getModelViewMatrix() const;

	virtual void render(Viewer& viewer) = 0;
	virtual void update() = 0;

	void transform(const glm::vec3& dstate);

protected:
	glm::vec3 m_state;
	std::shared_ptr<MeshRenderable> m_mesh_renderable;
	Shader m_shader;
};

typedef std::shared_ptr<Solid> SolidPtr;

class Rectangle : public Solid
{
public:
	Rectangle(const glm::vec2& position, float width, float height, float angle = 0.0, const glm::vec3& color = glm::vec3(0.0, 0.0, 0.0));

	virtual void render(Viewer& viewer);
	virtual void update();
private:
	float m_width, m_height;
	glm::vec3 m_color;
};