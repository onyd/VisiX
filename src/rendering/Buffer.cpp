#include "Buffer.h"

namespace vx
{

Buffer::Buffer(GLuint buffer_type) 
	: m_buffer_type(buffer_type)
{
	glGenBuffers(1, &m_id);
}

} // namespace vx
