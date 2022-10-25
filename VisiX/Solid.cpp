#include "Solid.h"

#include "Viewer.h"
#include "Attribute.h"

Solid::Solid(const glm::vec2& position, float angle) 
	: m_state(glm::vec3(position, angle)), m_shader(Shader::from_file("default.vert", "default.frag"))
{
}

glm::mat4 Solid::getModelViewMatrix() const
{
	return glm::mat4(cos(m_state.z), -sin(m_state.z), 0.0, m_state.x,
					 sin(m_state.z), cos(m_state.z), 0.0, m_state.y,
					 0.0, 0.0, 1.0, 0.0,
					 0.0, 0.0, 0.0, 1.0);
}

void Solid::transform(const glm::vec3& dstate)
{
	m_state += dstate;
}

Rectangle::Rectangle(const glm::vec2& position, float width, float height, float angle, const glm::vec3& color)
	: Solid(position, angle), m_width(width), m_height(height), m_color(color)
{
	std::vector<Vertex> verticies = {
		{ Vertex{ glm::vec3(-m_width / 2.0f, -m_height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), m_color, glm::vec2(0.0f, 0.0f)} },
		{ Vertex{ glm::vec3(+m_width / 2.0f, -m_height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), m_color, glm::vec2(0.0f, 1.0f)} },
		{ Vertex{ glm::vec3(+m_width / 2.0f, +m_height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), m_color, glm::vec2(1.0f, 1.0f)} },
		{ Vertex{ glm::vec3(-m_width / 2.0f, +m_height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), m_color, glm::vec2(1.0f, 0.0f)} }
	};
	std::vector<GLuint> indicies = { 0, 1, 2,
								     0, 2, 3 };

	MeshPtr mesh = std::make_shared<Mesh>(verticies, indicies, std::vector<Texture>());

	glm::mat4 object_model = glm::mat4(1.0f);
	glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 light_pos = glm::vec3(0.5f, 0.5f, 1.0f);

	std::initializer_list<std::pair<const Shader&, std::shared_ptr<BaseAttribute>>> attributes = 
			{ { m_shader, std::make_shared<Mat4Attribute>("model", object_model) },
			  { m_shader, std::make_shared<Vec4Attribute>("lightColor", light_color) },
			  { m_shader, std::make_shared<Vec3Attribute>("lightPos", light_pos) } };
	m_mesh_renderable = std::make_shared<MeshRenderable>(mesh,m_shader,attributes);
}

void Rectangle::render(Viewer& viewer)
{
	viewer.addRenderable(m_mesh_renderable);
}

void Rectangle::update()
{
	for (auto& v : m_mesh_renderable->getMesh().verticies()) {
		v.m_position = glm::vec3(glm::vec4(v.m_position, 1.0) * getModelViewMatrix());
	}
	m_mesh_renderable->getMesh().update();
}
