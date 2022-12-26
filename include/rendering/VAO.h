#pragma once

#include <glad/glad.h>
#include "GLObject.h"
#include "Buffer.h"

namespace vx 
{

class VAO : public GLObject {
public:

	/**
	 * @fn	VAO::VAO();
	 *
	 * @brief	Default constructor
	 */
	VAO();

	/**
	 * @fn	void VAO::linkAttribute(VBO vbo, GLuint layout, GLuint nComponents, GLenum type, GLsizeiptr stride, void* offset);
	 *
	 * @brief	Links an attribute
	 *
	 * @param 		  	buffer		   	The buffer.
	 * @param 		  	layout	   		The layout.
	 * @param 		  	n_components	The components.
	 * @param 		  	type	   		The type.
	 * @param 		  	stride	   		The stride.
	 * @param [in,out]	offset	   		If non-null, the offset.
	 */
	void linkAttribute(const Buffer& buffer, GLuint layout, GLuint n_components, GLenum type, GLsizeiptr stride, void* offset) const;

	/**
	 * @fn	virtual void VAO::bind();
	 *
	 * @brief	Binds this object to the current context
	 */
	virtual void bind() const;

	/**
	 * @fn	virtual void VAO::unbind();
	 *
	 * @brief	Unbinds this object of the current context
	 */
	virtual void unbind() const;

	/**
	 * @fn	virtual void VAO::destroy();
	 *
	 * @brief	Destroys this object
	 */
	virtual void destroy();

};

}