#pragma once

#include <vector>
#include <memory>
#include <glad/glad.h>

#include "Renderable.h"
#include "Texture.h"
#include "Drawer.h"

namespace vx 
{

class TextureManager
{
public:
	TextureManager(const std::vector<Texture> textures);

	virtual void activate(const Shader& shader) const;

private:
	std::vector<Texture> m_textures;
};

typedef std::shared_ptr<TextureManager> TextureManagerPtr;

class Mesh : public Renderable
{
public:
	Mesh(const std::vector<Vertex>& vertices,
		 const Shader& shader,
		 GLuint mode = GL_TRIANGLES);
	
	Mesh(const std::vector<Vertex>& vertices,
		 const std::vector<GLuint>& indices,
		 const Shader& shader,
		 GLuint mode = GL_TRIANGLES);

	Mesh(const std::vector<Vertex>& vertices, 
		 const std::vector<GLuint>& indices, 
		 const std::vector<Texture>& textures, 
		 const Shader& shader,
		 GLuint mode = GL_TRIANGLES);

	virtual void draw(const Camera& camera, float dt) const;

	void setVerticies(const std::vector<Vertex>& verticies);
	void setIndicies(const std::vector<GLuint>& indicies);

private:
	VertexDrawerPtr m_vertex_drawer = nullptr;
	TextureManagerPtr m_texture_drawer = nullptr;
};

typedef std::shared_ptr<Mesh> MeshPtr;

}