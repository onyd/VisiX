#pragma once 

#include "ShaderClass.h"
#include "Mesh.h"

class Camera;
class BaseAttribute;

class Renderable
{
public:
	Renderable(std::initializer_list<std::pair<const Shader&, std::shared_ptr<BaseAttribute>>> attributes);
	virtual void draw(const Camera& camera) const = 0;

protected:
	std::vector<std::pair<const Shader&, std::shared_ptr<BaseAttribute>>> m_attributes;
};

typedef std::shared_ptr<Renderable> RenderablePtr;

class MeshRenderable : public Renderable
{
public:
	MeshRenderable(const MeshPtr& mesh, const Shader& shader, std::initializer_list<std::pair<const Shader&, std::shared_ptr<BaseAttribute>>> attributes);
	virtual void draw(const Camera& camera) const;

	Mesh& getMesh();
private:
	MeshPtr m_mesh;
	Shader m_shader;
};