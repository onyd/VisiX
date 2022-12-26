#include "Drawer.h"

namespace vx
{
    
VertexDrawer::VertexDrawer(const std::vector<Vertex>& verticies, GLuint mode)
	: m_vbo(verticies), m_verticies_size(verticies.size()), m_mode(mode)
{
	m_vao.bind();
	m_vbo.bind();

	// Link the VAO to the VBO
	m_vao.linkAttribute(m_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_vao.linkAttribute(m_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	m_vao.linkAttribute(m_vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	m_vao.linkAttribute(m_vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	m_vao.unbind();
	m_vbo.unbind();
}

VertexDrawer::~VertexDrawer()
{
	m_vao.destroy();
	m_vbo.destroy();
}

void VertexDrawer::setVerticies(const std::vector<Vertex>& verticies)
{
	m_vbo.setData(verticies, GL_STATIC_DRAW);
	m_verticies_size = verticies.size();
}

ArrayDrawer::ArrayDrawer(const std::vector<Vertex>& verticies, GLuint mode)
	: VertexDrawer(verticies, mode)
{}

void ArrayDrawer::draw(const Shader& shader, const Camera& camera) const
{
	shader.activate();
	m_vao.bind();
	m_vbo.bind();

	shader.update("cameraMatrix", camera.viewMatrix());

	glDrawArrays(m_mode, 0, m_verticies_size);
	m_vao.unbind();
	m_vbo.unbind();
}

bool ArrayDrawer::isElementDrawer() const
{
	return false;
}

const ElementDrawer& ArrayDrawer::asElementDrawer() const
{
	throw std::runtime_error("ArrayDrawer can't be casted as ElementDrawer.");
}

ElementDrawer& ArrayDrawer::asElementDrawer()
{
	throw std::runtime_error("ArrayDrawer can't be casted as ElementDrawer.");
}

ElementDrawer::ElementDrawer(const std::vector<Vertex>& verticies, const std::vector<GLuint> indicies, GLuint mode)
	: VertexDrawer(verticies, mode), m_ebo(indicies), m_indicies_size(indicies.size())
{}

ElementDrawer::~ElementDrawer()
{
	m_ebo.destroy();
}

void ElementDrawer::draw(const Shader& shader, const Camera& camera) const
{
	shader.activate();
	m_vao.bind();
	m_ebo.bind();

	shader.update("cameraMatrix", camera.viewMatrix());

	glDrawElements(m_mode, m_indicies_size, GL_UNSIGNED_INT, 0);
	m_vao.unbind();
	m_ebo.unbind();
}

void ElementDrawer::setIndicies(const std::vector<GLuint>& indicies)
{
	m_ebo.setData(indicies, GL_STATIC_DRAW);
}

bool ElementDrawer::isElementDrawer() const
{
	return true;
}

const ElementDrawer& ElementDrawer::asElementDrawer() const
{
	return static_cast<const ElementDrawer&>(*this);
}

ElementDrawer& ElementDrawer::asElementDrawer()
{
	return static_cast<ElementDrawer&>(*this);
}

} // namespace vx
