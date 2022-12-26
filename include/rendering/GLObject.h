#pragma once

#include <glad/glad.h>

namespace vx 
{

class GLObject 
{
public:
	GLuint m_id;

	inline virtual void destroy() = 0;

	inline virtual void bind() const = 0;
	inline virtual void unbind() const = 0;
};

}