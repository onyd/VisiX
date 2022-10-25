#include "Attribute.h"

BaseAttribute::BaseAttribute(const char* identifier) : m_identifier(identifier)
{
}

FloatAttribute::FloatAttribute(const char* identifier, float value) : BaseAttribute(identifier), m_value(value)
{
}

void FloatAttribute::exportAsUniform(const ShaderBase& shader) const
{
	shader.activate();
	glUniform1f(glGetUniformLocation(shader.m_id, m_identifier), m_value);
}

IntAttribute::IntAttribute(const char* identifier, int value) : BaseAttribute(identifier), m_value(value)
{
}

void IntAttribute::exportAsUniform(const ShaderBase& shader) const
{
	shader.activate();
	glUniform1i(glGetUniformLocation(shader.m_id, m_identifier), m_value);
}

Vec2Attribute::Vec2Attribute(const char* identifier, const glm::vec2& value) : BaseAttribute(identifier), m_value(value)
{
}

void Vec2Attribute::exportAsUniform(const ShaderBase& shader) const
{
	shader.activate();
	glUniform2f(glGetUniformLocation(shader.m_id, m_identifier), m_value.x, m_value.y);
}

Vec3Attribute::Vec3Attribute(const char* identifier, const glm::vec3& value) : BaseAttribute(identifier), m_value(value)
{
}

void Vec3Attribute::exportAsUniform(const ShaderBase& shader) const
{
	shader.activate();
	glUniform3f(glGetUniformLocation(shader.m_id, m_identifier), m_value.x, m_value.y, m_value.z);
}

Vec4Attribute::Vec4Attribute(const char* identifier, const glm::vec4& value) : BaseAttribute(identifier), m_value(value)
{
}

void Vec4Attribute::exportAsUniform(const ShaderBase& shader) const
{
	shader.activate();
	glUniform4f(glGetUniformLocation(shader.m_id, m_identifier), m_value.x, m_value.y, m_value.z, m_value.w);
}

Mat2Attribute::Mat2Attribute(const char* identifier, const glm::mat2& value) : BaseAttribute(identifier), m_value(value)
{
}

void Mat2Attribute::exportAsUniform(const ShaderBase& shader) const
{
	shader.activate();
	glUniformMatrix2fv(glGetUniformLocation(shader.m_id, m_identifier), 1, GL_FALSE, glm::value_ptr(m_value));
}

Mat3Attribute::Mat3Attribute(const char* identifier, const glm::mat3& value) : BaseAttribute(identifier), m_value(value)
{
}

void Mat3Attribute::exportAsUniform(const ShaderBase& shader) const
{
	shader.activate();
	glUniformMatrix3fv(glGetUniformLocation(shader.m_id, m_identifier), 1, GL_FALSE, glm::value_ptr(m_value));
}

Mat4Attribute::Mat4Attribute(const char* identifier, const glm::mat4& value) : BaseAttribute(identifier), m_value(value)
{
}

void Mat4Attribute::exportAsUniform(const ShaderBase& shader) const
{
	shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.m_id, m_identifier), 1, GL_FALSE, glm::value_ptr(m_value));
}


