#include "EBO.h"

EBO::EBO(const std::vector<GLuint>& indices) : Buffer(indices, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER)
{
}

void EBO::destroy() 
{
	glDeleteBuffers(1, &m_id);
}