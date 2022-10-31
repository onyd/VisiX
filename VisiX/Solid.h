#pragma once

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Renderable.h"

class Viewer;

struct State
{
	glm::vec2 m_position;
	float m_angle;
};

class Solid
{
public:
	Solid(const glm::vec2& position, float angle);

	glm::mat4 getModelViewMatrix() const;

	virtual void render(Viewer& viewer) = 0;
	virtual void update() = 0;

	void translate(const glm::vec2& t);
	void rotate(float a);

protected:
	State m_state;
	std::vector<glm::vec3> m_verticies;
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