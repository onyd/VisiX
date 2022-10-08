#include "ShaderClass.h"
#include "Camera.h"
#include "Mesh.h"
#include "Attribute.h"

class Renderable
{
public:
	Renderable(std::initializer_list<std::pair<const Shader&, std::shared_ptr<BaseAttribute>>> attributes);
	virtual void draw(const Camera& camera) const = 0;

protected:
	std::vector<std::pair<const Shader&, std::shared_ptr<BaseAttribute>>> m_attributes;
};

class MeshRenderable : public Renderable
{
public:
	MeshRenderable(const Mesh& mesh, const Shader& shader, std::initializer_list<std::pair<const Shader&, std::shared_ptr<BaseAttribute>>> attributes);
	virtual void draw(const Camera& camera) const;
private:
	Mesh m_mesh;
	Shader m_shader;
};