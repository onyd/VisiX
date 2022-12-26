#include "Texture.h"

namespace vx 
{

Texture::Texture(const char* filename, const char* texture_type, GLuint slot, GLenum image_format, GLenum pixel_format, GLenum s_wrap, GLenum t_wrap, GLenum min_filter, GLenum mag_filter, GLenum pixel_type)
	: m_type(texture_type), m_slot(slot)
{
	// Load texture
	int width, height, nChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(filename, &width, &height, &nChannel, 0);

	// Generate OpenGL texture
	glGenTextures(1, &m_id);
	glActiveTexture(GL_TEXTURE0 + slot); // Use slot as texture slot
	glBindTexture(GL_TEXTURE_2D, m_id); // Make it the current texture in context

	// Set texture interpolation type
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

	// Set mapping type
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_wrap);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, image_format, width, height, 0, pixel_format, pixel_type, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Image data are no longer necessary 
	stbi_image_free(bytes);

	unbind();
}

void Texture::exportAsUniform(const Shader& shader, const char* uniform, GLuint unit) const
{
	GLuint tex_uniform_loc = glGetUniformLocation(shader.m_id, uniform);
	shader.activate();
	glUniform1i(tex_uniform_loc, unit);
}

void Texture::bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::destroy()
{
	glDeleteTextures(1, &m_id);
}

}