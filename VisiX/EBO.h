#pragma once

#include <vector>
#include <glad/glad.h>

#include "GLObject.h"

class EBO : public GLObject {
public:
	/**
	 * @fn	EBO::EBO(std::vector<GLuint>& indices);
	 *
	 * @summary	Constructor.
	 *
	 * @param [in]	indices	If non-null, the indices.
	 *
	 * ### param 	 	size	The size.
	 */
	EBO(std::vector<GLuint>& indices);

	/**
	 * @fn	virtual void EBO::bind();
	 *
	 * @brief	Binds this object to the current context
	 */
	virtual void bind() const;

	/**
	 * @fn	virtual void EBO::unbind();
	 *
	 * @brief	Unbinds this object of the current context
	 */
	virtual void unbind() const;

	/**
	 * @fn	virtual void EBO::destroy();
	 *
	 * @brief	Destroys this object
	 */
	virtual void destroy();
};