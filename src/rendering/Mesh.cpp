#include "Mesh.h"

namespace vx 
{

Mesh::Mesh(const std::vector<Vertex>& vertices, 
		   const Shader& shader,
		   GLuint mode)
	: Renderable(shader), m_vertex_drawer(std::make_shared<ArrayDrawer>(vertices, mode))
{}

Mesh::Mesh(const std::vector<Vertex>& vertices, 
		   const std::vector<GLuint>& indices, 
		   const Shader& shader,
		   GLuint mode)
	: Renderable(shader), m_vertex_drawer(std::make_shared<ElementDrawer>(vertices, indices, mode))
{}

Mesh::Mesh(const std::vector<Vertex>& vertices,
			const std::vector<GLuint>& indices, 
		    const std::vector<Texture>& textures,
			const Shader& shader,
		    GLuint mode)
	: Renderable(shader), m_vertex_drawer(std::make_shared<ElementDrawer>(vertices, indices, mode)), m_texture_drawer(std::make_shared<TextureManager>(textures))
{}

void Mesh::draw(const Camera& camera, float dt) const
{
	for (const auto& [identifier, uniform] : m_uniforms)
        uniform->update(identifier, m_shader);

	if (m_texture_drawer != nullptr)
		m_texture_drawer->activate(m_shader);
	if (m_vertex_drawer != nullptr)
		m_vertex_drawer->draw(m_shader, camera);
}

void Mesh::setVerticies(const std::vector<Vertex>& verticies)
{
	if (m_vertex_drawer != nullptr)
		m_vertex_drawer->setVerticies(verticies);
}

void Mesh::setIndicies(const std::vector<GLuint>& indicies)
{
	if (m_vertex_drawer != nullptr && m_vertex_drawer->isElementDrawer())
		m_vertex_drawer->asElementDrawer().setIndicies(indicies);
}

TextureManager::TextureManager(const std::vector<Texture> textures)
	: m_textures(textures)
{}

void TextureManager::activate(const Shader& shader) const
{
	unsigned int n_diffuse = 0;
	unsigned int n_specular = 0;

	for (int i = 0; i < m_textures.size(); i++) {
		std::string num;
		std::string type = m_textures[i].m_type;

		if (type == "diffuse") {
			num = std::to_string(n_diffuse++);
		}
		else if (type == "specular") {
			num = std::to_string(n_specular++);
		}
		m_textures[i].exportAsUniform(shader, (type + num).c_str(), i);
		m_textures[i].bind();
	}
}

}