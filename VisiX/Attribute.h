#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderClass.h"

class BaseAttribute
{
public:
	BaseAttribute(const char* identifier);
	virtual void exportAsUniform(const ShaderBase& shader) const = 0;
protected:
	const char* m_identifier;
};

class FloatAttribute : public BaseAttribute
{
public:
	FloatAttribute(const char* identifier, float value);
	virtual void exportAsUniform(const ShaderBase& shader) const;

private:
	float m_value;
};

class IntAttribute : public BaseAttribute
{
public:
	IntAttribute(const char* identifier, int value);
	virtual void exportAsUniform(const ShaderBase& shader) const;

private:
	int m_value;
};

class Vec2Attribute : public BaseAttribute
{
public:
	Vec2Attribute(const char* identifier, const glm::vec2& value);
	virtual void exportAsUniform(const ShaderBase& shader) const;

private:
	glm::vec2 m_value;
};

class Vec3Attribute : public BaseAttribute
{
public:
	Vec3Attribute(const char* identifier, const glm::vec3& value);
	virtual void exportAsUniform(const ShaderBase& shader) const;

private:
	glm::vec3 m_value;
};

class Vec4Attribute : public BaseAttribute
{
public:
	Vec4Attribute(const char* identifier, const glm::vec4& value);
	virtual void exportAsUniform(const ShaderBase& shader) const;

private:
	glm::vec4 m_value;
};

class Mat2Attribute : public BaseAttribute
{
public:
	Mat2Attribute(const char* identifier, const glm::mat2& value);
	virtual void exportAsUniform(const ShaderBase& shader) const;

private:
	glm::mat2 m_value;
};

class Mat3Attribute : public BaseAttribute
{
public:
	Mat3Attribute(const char* identifier, const glm::mat3& value);
	virtual void exportAsUniform(const ShaderBase& shader) const;

private:
	glm::mat3 m_value;
};

class Mat4Attribute : public BaseAttribute
{
public:
	Mat4Attribute(const char* identifier, const glm::mat4& value);
	virtual void exportAsUniform(const ShaderBase& shader) const;

private:
	glm::mat4 m_value;
};

