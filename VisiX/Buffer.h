#pragma once

#include <vector>

#include "GLObject.h"

class Buffer : public GLObject
{
public:
	template <class T>
	Buffer(const std::vector<T>& initial_data, GLuint mode = GL_STATIC_DRAW);

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

};

template <class T>
Buffer::Buffer(const std::vector<T>& initial_data, GLuint mode)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, initial_data.size() * sizeof(T), &initial_data[0], mode);
}

void Buffer::destroy()
{
	glDeleteBuffers(1, &m_id);
}

void Buffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void Buffer::bindBase(GLuint location) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, m_id);
}

void Buffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::unbindBase(GLuint location) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, 0);
}