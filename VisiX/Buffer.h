#pragma once

#include <vector>

#include "GLObject.h"

class Buffer : public GLObject
{
public:
	template <class T>
	Buffer(const std::vector<T>& initial_data, GLuint mode = GL_STATIC_DRAW, GLuint buffer_type = GL_ARRAY_BUFFER);

	template <class T>
	void setData(const std::vector<T>& data, GLuint mode) const;

	/**
	 * @fn	inline virtual void Buffer::destroy();
	 *
	 * @brief	Destroys this object
	 */
	inline virtual void destroy();

	/**
	 * @fn	inline virtual void Buffer::bind() const;
	 *
	 * @brief	Binds this object to the current context
	 */
	inline virtual void bind() const;

	/**
	 * @fn	inline virtual void Buffer::bindBase(GLuint location) const;
	 *
	 * @brief	Binds this object to the current context
	 */
	inline virtual void bindBase(GLuint location) const;

	/**
	 * @fn	inline virtual void Buffer::unbind() const;
	 *
	 * @brief	Unbinds this object of the current context
	 */
	inline virtual void unbind() const;

	/**
	 * @fn	inline virtual void Buffer::unbindBase() const;
	 *
	 * @brief	Unbinds this object of the current context
	 */
	inline virtual void unbindBase(GLuint location) const;
protected:
	GLuint m_buffer_type;
};

template <class T>
Buffer::Buffer(const std::vector<T>& initial_data, GLuint mode, GLuint buffer_type) : m_buffer_type(buffer_type)
{
	glGenBuffers(1, &m_id);
	setData(initial_data, mode);
}

template <class T>
void Buffer::setData(const std::vector<T>& data, GLuint mode) const
{
	bind();
	glBufferData(m_buffer_type, data.size() * sizeof(T), &data[0], mode);
	unbind();
}

void Buffer::destroy()
{
	glDeleteBuffers(1, &m_id);
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