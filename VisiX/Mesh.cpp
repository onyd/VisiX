#include "Mesh.h"
#include "Camera.h"


Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures)
	: m_vertices(vertices), m_indices(indices), m_textures(textures), m_vbo(vertices), m_ebo(indices)
{
	// Create the vertex array
	m_vao.bind();
	m_vbo.bind();
	m_ebo.bind();

	// Create the vertex buffer and the indices buffer

	// Link the VAO to the VBO
	m_vao.linkAttribute(m_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_vao.linkAttribute(m_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	m_vao.linkAttribute(m_vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	m_vao.linkAttribute(m_vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	// unbind all
	m_vao.unbind();
	m_vbo.unbind();
	m_ebo.unbind();
}

void Mesh::draw(const Shader& shader, const Camera& camera) const
{
	shader.activate();
	m_vao.bind();

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

	camera.exportPosition(shader, "cameraPos");
	camera.exportMatrix(shader, "cameraMatrix");

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::update() const
{
	m_vbo.setData(m_vertices, GL_STATIC_DRAW);
}

const std::vector<Vertex>& Mesh::verticies() const
{
	return m_vertices;
}

std::vector<Vertex>& Mesh::verticies()
{
	return m_vertices;
}
