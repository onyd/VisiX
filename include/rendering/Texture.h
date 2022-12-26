#pragma once

#include "stb/stb_image.h"

#include <glad/glad.h>
#include "GLObject.h"
#include "ShaderClass.h"

namespace vx 
{

class Texture : public GLObject {
public:
	const char* m_type;
	GLuint m_slot;

	/**
	 * @fn	Texture::Texture(const char* filename, const char* texture_type, GLuint slot=0, GLenum image_format = GL_RGBA, GLenum pixel_format = GL_RGBA, GLenum s_wrap=GL_REPEAT, GLenum t_wrap = GL_REPEAT, GLenum min_filter = GL_NEAREST, GLenum mag_filter = GL_NEAREST, GLenum pixel_type=GL_UNSIGNED_BYTE);
	 *
	 * @brief	Constructor
	 *
	 * @param 	filename		Filename of the file.
	 * @param 	texture_type	The texture type in {'diffuse', 'specular'}.
	 * @param 	slot			(Optional) The slot of the texture.
	 * @param 	image_format	(Optional) The channel format of the image.
	 * @param 	pixel_format	(Optional) The pixel format of the texture.
	 * @param 	s_wrap			(Optional) The wrap mode for x axis.
	 * @param 	t_wrap			(Optional) The wrap mode for y axis.
	 * @param 	min_filter  	(Optional) A filter specifying the minimum.
	 * @param 	mag_filter  	(Optional) A filter specifying the magnitude.
	 * @param 	pixel_type  	(Optional) Data type of the pixel.
	 */
	Texture(const char* filename, const char* texture_type, GLuint slot=0, GLenum image_format = GL_RGBA, GLenum pixel_format = GL_RGBA, GLenum s_wrap=GL_REPEAT, GLenum t_wrap = GL_REPEAT, GLenum min_filter = GL_NEAREST_MIPMAP_LINEAR, GLenum mag_filter = GL_NEAREST, GLenum pixel_type=GL_UNSIGNED_BYTE);

	/**
	 * @fn	void Texture::exportAsUniform(Shader& shader, const char* uniform, GLuint unit);
	 *
	 * @brief	Sets the texture as uniform variable with the name "uniform" in the shader
	 *
	 * @param [in]	shader 	The shader.
	 * @param 		  	uniform	The uniform.
	 * @param 		  	unit   	The unit.
	 */
	void exportAsUniform(const Shader& shader, const char* uniform, GLuint unit) const;

	/**
	 * @fn	virtual void Texture::bind();
	 *
	 * @brief	Binds this object to the current context
	 */
	virtual void bind() const;

	/**
	 * @fn	virtual void Texture::unbind();
	 *
	 * @brief	Unbinds this object of the current context
	 */
	virtual void unbind() const;

	/**
	 * @fn	virtual void Texture::destroy();
	 *
	 * @brief	Destroys this object
	 */
	virtual void destroy();
};

}