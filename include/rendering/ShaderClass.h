#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include "Buffer.h"

namespace vx 
{

std::string getFileContents(const char* filename);

class ShaderBase
{
public:
	ShaderBase();
	ShaderBase(const ShaderBase& shader);
	ShaderBase& operator=(const ShaderBase& shader);
	~ShaderBase();

	GLuint m_id = -1;

	virtual void activate() const = 0;

	void update(const char* uniform_identifier, float value) const;
	void update(const char* uniform_identifier, int value) const;

	template <class T>
	void update(const char* uniform_identifier, const T& value) const;

protected:
	int* m_counter;
};

inline void ShaderBase::update(const char* uniform_identifier, float value) const
{
	activate();
	glUniform1f(glGetUniformLocation(m_id, uniform_identifier), value);
}

inline void ShaderBase::update(const char* uniform_identifier, int value) const
{
	activate();
	glUniform1i(glGetUniformLocation(m_id, uniform_identifier), value);
}

template<>
inline void ShaderBase::update<glm::vec2>(const char* uniform_identifier, const glm::vec2& value) const
{
	activate();
	glUniform2f(glGetUniformLocation(m_id, uniform_identifier), value.x, value.y);
}

template<>
inline void ShaderBase::update<glm::vec3>(const char* uniform_identifier, const glm::vec3& value) const
{
	activate();
	glUniform3f(glGetUniformLocation(m_id, uniform_identifier), value.x, value.y, value.z);
}

template<>
inline void ShaderBase::update<glm::vec4>(const char* uniform_identifier, const glm::vec4& value) const
{
	activate();
	glUniform4f(glGetUniformLocation(m_id, uniform_identifier), value.x, value.y, value.z, value.w);
}

template<>
inline void ShaderBase::update<glm::mat2>(const char* uniform_identifier, const glm::mat2& value) const
{
	activate();
	glUniformMatrix2fv(glGetUniformLocation(m_id, uniform_identifier), 1, GL_FALSE, glm::value_ptr(value));
}

template<>
inline void ShaderBase::update<glm::mat3>(const char* uniform_identifier, const glm::mat3& value) const
{
	activate();
	glUniformMatrix3fv(glGetUniformLocation(m_id, uniform_identifier), 1, GL_FALSE, glm::value_ptr(value));
}

template<>
inline void ShaderBase::update<glm::mat4>(const char* uniform_identifier, const glm::mat4& value) const
{
	activate();
	glUniformMatrix4fv(glGetUniformLocation(m_id, uniform_identifier), 1, GL_FALSE, glm::value_ptr(value));
}

class Shader : public ShaderBase
{
public:
	Shader(const char* vertex_source,
		const char* fragment_source);

	static Shader from_file(const char* vertex_file,
		const char* fragment_file);

	virtual void activate() const;

private:
	void compileErrors(unsigned int shader, const char* type);
};

class ComputeShader : public ShaderBase
{
public:

	ComputeShader(const char* source, const std::vector<Buffer>& buffers);

	static ComputeShader from_file(const char* source_file, const std::vector<Buffer>& buffers);

	virtual void activate() const;
	void dispatch(GLuint group_size_x, GLuint group_size_y, GLuint group_size_z) const;

private:
	std::vector<Buffer> m_buffers;
};


}