#pragma once

#include <string>
#include <vector>

#include "Vertex.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"

class Camera;
class Shader;

class Mesh
{
public:

	/**
		* @fn	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures);
		*
		* @brief	Constructor.
		*
		* @param 	vertices	The vertices.
		* @param 	indices 	The indices.
		* @param 	textures	The textures.
		*/
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures);

	/**
		* @fn	virtual void Mesh::draw(Shader& shader, Camera& camera);
		*
		* @brief	Draws the Mesh.
		*
		* @param [in]	shader	The shader.
		* @param [in]	camera	The camera.
		*/
	virtual void draw(const Shader& shader, const Camera& camera) const;

	void update() const;

	const std::vector<Vertex>& verticies() const;
	std::vector<Vertex>& verticies();

private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;

	VAO m_vao;
	Buffer m_vbo;
	EBO m_ebo;
};

typedef std::shared_ptr<Mesh> MeshPtr;