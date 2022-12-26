#pragma once

#include <vector>

#include <glad/glad.h>
#include "GLObject.h"

namespace vx 
{

class Buffer : public GLObject
{
public:
	Buffer(GLuint buffer_type = GL_ARRAY_BUFFER);

	template <class T>
	Buffer(const std::vector<T>& initial_data, GLuint mode = GL_STATIC_DRAW, GLuint buffer_type = GL_ARRAY_BUFFER);
	
	template <class T>
	void setData(const std::vector<T>& data, GLuint mode) const;

	inline virtual void destroy();

	inline size_t size() const;

	inline virtual void bind() const;
	inline virtual void bindBase(GLuint location) const;
	inline virtual void unbind() const;
	inline virtual void unbindBase(GLuint location) const;

protected:
	GLuint m_buffer_type;
	mutable size_t m_size = 0;
};

template <class T>
Buffer::Buffer(const std::vector<T>& initial_data, GLuint mode, GLuint buffer_type) 
	: Buffer(buffer_type)
{
	setData(initial_data, mode);
}

template <class T>
void Buffer::setData(const std::vector<T>& data, GLuint mode) const
{
	bind();
	glBufferData(m_buffer_type, data.size() * sizeof(T), data.data(), mode);
	m_size = data.size();
	unbind();
}

void Buffer::destroy()
{
	glDeleteBuffers(1, &m_id);
}

size_t Buffer::size() const
{
	return m_size;
}

void Buffer::bind() const
{
	glBindBuffer(m_buffer_type, m_id);
}

void Buffer::bindBase(GLuint location) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, m_id);
}

void Buffer::unbind() const
{
	glBindBuffer(m_buffer_type, 0);
}

void Buffer::unbindBase(GLuint location) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, 0);
}

}