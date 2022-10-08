#include "EBO.h"

EBO::EBO(std::vector<GLuint>& indices) 
{
	glGenBuffers(1, &m_id); // Create our indicies buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); // Make it the current indicies buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW); // Load indicies data
}

void EBO::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void EBO::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::destroy() 
{
	glDeleteBuffers(1, &m_id);
}