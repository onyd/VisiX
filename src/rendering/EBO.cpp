#include "EBO.h"

namespace vx 
{

EBO::EBO(const std::vector<GLuint>& indices) 
	: Buffer(indices, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER)
{}


}