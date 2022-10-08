#pragma once

#include <string>
#include <vector>

#include "Vertex.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Node.h"


class Mesh : public TransformNode {
	public:

		/**
		 * @fn	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
		 *
		 * @brief	Constructor.
		 *
		 * @param 	vertices	The vertices.
		 * @param 	indices 	The indices.
		 * @param 	textures	The textures.
		 */
		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

		/**
		 * @fn	virtual void Mesh::draw(Shader& shader, Camera& camera);
		 *
		 * @brief	Draws the Mesh.
		 *
		 * @param [in]	shader	The shader.
		 * @param [in]	camera	The camera.
		 */
		virtual void draw(const Shader& shader, const Camera& camera) const;

	private:
		std::vector<Vertex> m_vertices;
		std::vector<GLuint> m_indices;
		std::vector<Texture> m_textures;

		VAO vao;
};
