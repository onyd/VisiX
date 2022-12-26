#pragma once

#include <vector>
#include <memory>
#include <glad/glad.h>

#include "Vertex.h"
#include "VAO.h"
#include "EBO.h"
#include "ShaderClass.h"
#include "Camera.h"

namespace vx
{
    
class ElementDrawer;

class VertexDrawer
{
public:
	VertexDrawer(const std::vector<Vertex>& verticies, GLuint mode);
	~VertexDrawer();

	virtual void draw(const Shader& shader, const Camera& camera) const = 0;
	
	void setVerticies(const std::vector<Vertex>& verticies);

	virtual bool isElementDrawer() const = 0;
	virtual const ElementDrawer& asElementDrawer() const = 0;
	virtual ElementDrawer& asElementDrawer() = 0;

protected:
	VAO m_vao;
	Buffer m_vbo;

	GLuint m_verticies_size;
	GLuint m_mode;
};

typedef std::shared_ptr<VertexDrawer> VertexDrawerPtr;

class ArrayDrawer : public VertexDrawer
{
public:
	ArrayDrawer(const std::vector<Vertex>& verticies, GLuint mode);

	virtual void draw(const Shader& shader, const Camera& camera) const;

	virtual bool isElementDrawer() const;
	virtual const ElementDrawer& asElementDrawer() const;
	virtual ElementDrawer& asElementDrawer();
};

class ElementDrawer : public VertexDrawer
{
public:
	ElementDrawer(const std::vector<Vertex>& verticies, const std::vector<GLuint> indicies, GLuint mode);
	~ElementDrawer();

	virtual void draw(const Shader& shader, const Camera& camera) const;

	void setIndicies(const std::vector<GLuint>& indicies);

	virtual bool isElementDrawer() const;
	virtual const ElementDrawer& asElementDrawer() const;
	virtual ElementDrawer& asElementDrawer();

private:
	EBO m_ebo;
	GLuint m_indicies_size;
};


} // namespace vx
