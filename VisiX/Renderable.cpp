#include "Renderable.h"


Renderable::Renderable(std::initializer_list<std::pair<const Shader&, std::shared_ptr<BaseAttribute>>> attributes) : m_attributes(attributes)
{
}

MeshRenderable::MeshRenderable(const Mesh& mesh, const Shader& shader, std::initializer_list<std::pair<const Shader&, std::shared_ptr<BaseAttribute>>> attributes) : Renderable(attributes), m_mesh(mesh), m_shader(shader)
{
}

void MeshRenderable::draw(const Camera& camera) const
{
	for (const auto& attribute : m_attributes) {
		attribute.second->exportAsUniform(attribute.first);
	}
	m_mesh.draw(m_shader, camera);
}