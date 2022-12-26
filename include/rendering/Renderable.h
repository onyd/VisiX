#pragma once 

#include <vector>
#include <memory>

#include "Camera.h"
#include "ShaderClass.h"
#include "Uniform.h"

namespace vx 
{

class Renderable
{
public:
	Renderable(const Shader& shader);
	virtual void draw(const Camera& camera, float dt) const = 0;

	template <class T>
    void setUniform(const char* identifier, const Uniform<T>& uniform);

protected:
	Shader m_shader;
    std::unordered_map<const char*, UniformBasePtr> m_uniforms;
};

template <class T>
void Renderable::setUniform(const char* identifier, const Uniform<T>& uniform)
{
    m_uniforms[identifier] = std::make_shared<Uniform<T>>(uniform);
} 

typedef std::shared_ptr<Renderable> RenderablePtr;

}