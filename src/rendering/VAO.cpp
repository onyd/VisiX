#include "VAO.h"

namespace vx 
{

VAO::VAO() 
{
	glGenVertexArrays(1, &m_id); // Create our vertex array
}

void VAO::destroy() 
{
	glDeleteVertexArrays(1, &m_id);
}

void VAO::linkAttribute(const Buffer& buffer, GLuint layout, GLuint n_components, GLenum type, GLsizeiptr stride, void* offset) const
{
	buffer.bind();
	glVertexAttribPointer(layout, n_components, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	buffer.unbind();
}

void VAO::bind() const
{
	glBindVertexArray(m_id);
}

void VAO::unbind() const
{
	glBindVertexArray(0);
}

}