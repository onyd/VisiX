#pragma once

#include <glad/glad.h>

class GLObject 
{
public:
	GLuint m_id;

	/**
	 * @fn	GLObject::GLObject();
	 *
	 * @brief	Default constructor
	 */
	GLObject();

	/**
	 * @fn	virtual void GLObject::destroy()=0;
	 *
	 * @brief	Destroys this object
	 */
	inline virtual void destroy() = 0;

	/**
	 * @fn	virtual void GLObject::bind() const=0;
	 *
	 * @brief	Binds this object to the current context
	 */
	inline virtual void bind() const = 0;

	/**
	 * @fn	virtual void GLObject::unbind() const=0;
	 *
	 * @brief	Unbinds this object of the current context
	 */
	inline virtual void unbind() const = 0;
};