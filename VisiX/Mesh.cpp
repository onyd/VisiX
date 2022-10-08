#include "Mesh.h"
#include "Camera.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
	: m_vertices(vertices), m_indices(indices), m_textures(textures)
{
	// Create the vertex array
	vao.bind();

	// Create the vertex buffer and the indices buffer
	Buffer vbo(vertices);
	EBO ebo(indices);

	// Link the VAO to the VBO
	vao.linkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.linkAttribute(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.linkAttribute(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.linkAttribute(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	// unbind all
	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}

void Mesh::draw(const Shader& shader, const Camera& camera) const
{
	shader.activate();
	vao.bind();

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
