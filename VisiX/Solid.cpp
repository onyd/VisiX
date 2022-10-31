#include "Solid.h"

#include "Viewer.h"
#include "Attribute.h"

Solid::Solid(const glm::vec2& position, float angle) 
	: m_state({ position, angle }), m_shader(Shader::from_file("default.vert", "default.frag"))
{
}

glm::mat4 Solid::getModelViewMatrix() const
{
	return glm::mat4(cos(m_state.m_angle), -sin(m_state.m_angle), 0.0, m_state.m_position.x,
					 sin(m_state.m_angle), cos(m_state.m_angle),  0.0, m_state.m_position.y,
					 0.0,				   0.0,				      1.0, 0.0,
					 0.0,				   0.0,					  0.0, 1.0);
}

void Solid::translate(const glm::vec2& t)
{
	m_state.m_position += t;
}

void Solid::rotate(float a)
{
	m_state.m_angle += a;
}

Rectangle::Rectangle(const glm::vec2& position, float width, float height, float angle, const glm::vec3& color)
	: Solid(position, angle), m_width(width), m_height(height), m_color(color)
{
	// Keep original model verticies
	m_verticies = { glm::vec3(-m_width / 2.0f, -m_height / 2.0f, 0.0f),
					glm::vec3(+m_width / 2.0f, -m_height / 2.0f, 0.0f),
					glm::vec3(+m_width / 2.0f, +m_height / 2.0f, 0.0f),
					glm::vec3(-m_width / 2.0f, +m_height / 2.0f, 0.0f) };

	// Initialize mesh
	std::vector<Vertex> verticies = {
		{ Vertex{ glm::vec3(-m_width / 2.0f, -m_height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), m_color} },
		{ Vertex{ glm::vec3(+m_width / 2.0f, -m_height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), m_color} },
		{ Vertex{ glm::vec3(+m_width / 2.0f, +m_height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), m_color} },
		{ Vertex{ glm::vec3(-m_width / 2.0f, +m_height / 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), m_color} }
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
	// Apply transformations (translation and rotation) and update mesh verticies
	auto& mesh_verticies = m_mesh_renderable->getMesh().verticies();
	for (int i = 0; i < mesh_verticies.size(); i++) {
		mesh_verticies[i].m_position = glm::vec3(glm::vec4(m_verticies[i], 1.0) * getModelViewMatrix());
	}
	m_mesh_renderable->getMesh().update();
}
