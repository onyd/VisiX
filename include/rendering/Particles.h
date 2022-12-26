#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Renderable.h"
#include "VAO.h"
#include "Buffer.h"
#include "Camera.h"
#include "ShaderClass.h"

namespace vx
{

struct Format
{
    size_t component_number, byte_size;
    GLenum type;
};

template <size_t Size, class T>
struct VertexFormat;

template <size_t Size>
struct VertexFormat<Size, float>
{
    static constexpr Format value = { Size, Size * sizeof(float), GL_FLOAT };
};

template <size_t Size>
struct VertexFormat<Size, int>
{
    static constexpr Format value = { Size, Size * sizeof(float), GL_INT };
};

class Particles : public Renderable
{
public:
    template <class T>
    Particles(const std::vector<T>& particles,
              const std::vector<Format> format,
              const Shader& shader,
              const std::string& compute_shader);

	virtual void draw(const Camera& camera, float dt) const;

private:
    VAO m_vao;
    Buffer m_vbo;

	ComputeShader m_compute_shader;
};

typedef std::shared_ptr<Particles> ParticlesPtr;

template <class T>
Particles::Particles(const std::vector<T>& particles,
                     const std::vector<Format> format,
                     const Shader& shader,
                     const std::string& compute_shader)
    : Renderable(shader), m_vbo(particles, GL_DYNAMIC_DRAW), m_compute_shader(ComputeShader::from_file(compute_shader.c_str(), {m_vbo}))
{
    m_vao.bind();
    size_t offset = 0;
    for (int i = 0; i < format.size(); i++) {
        m_vao.linkAttribute(m_vbo, i, format[i].component_number, format[i].type, format[i].byte_size, (void*)offset);
        offset += format[i].byte_size;
    }
    m_vao.unbind();
}

} // namespace vx
