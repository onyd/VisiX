#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ShaderClass.h"

namespace vx 
{

class UniformBase
{
public:
	virtual void update(const char* identifier, const Shader& shader) const = 0;
};

typedef std::shared_ptr<UniformBase> UniformBasePtr;

template <class T>
class Uniform: public UniformBase
{
public:
	Uniform(const T& value);
	virtual void update(const char* identifier, const Shader& shader) const;

private:
	T m_value;
};

template <class T>
Uniform<T>::Uniform(const T& value)
    : m_value(value)
{}

template <class T>
void Uniform<T>::update(const char* identifier, const Shader& shader) const
{
    shader.update(identifier, m_value);
}

}