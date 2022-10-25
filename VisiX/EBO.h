#pragma once

#include <vector>
#include <glad/glad.h>

#include "Buffer.h"

class EBO : public Buffer
{
public:
	/**
	 * @fn	EBO::EBO(const std::vector<GLuint>& indices);
	 *
	 * @summary	Constructor.
	 *
	 * @param [in]	indices	If non-null, the indices.
	 *
	 * ### param 	 	size	The size.
	 */
	EBO(const std::vector<GLuint>& indices);

	/**
	 * @fn	virtual void EBO::destroy();
	 *
	 * @brief	Destroys this object
	 */
	virtual void destroy();
};