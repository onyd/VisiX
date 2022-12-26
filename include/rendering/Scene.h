#pragma once

#include <vector>

#include <glad/glad.h>
#include "glfw3.h"
#include "Node.h"
#include "Light.h"
#include "Camera.h"

namespace vx 
{

class Scene 
{
public:
	Scene(const Camera& camera);

	void addLight(const LightPtr& light);
	void addNode(const NodePtr& node);

	void render(GLFWwindow* window, float dt);

	void resize(int width, int height);

private:
	Node m_root;
	std::vector<LightPtr> m_lights;

	Camera m_camera;
};

}