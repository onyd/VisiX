#pragma once

#include <vector>

#include "Node.h"
#include "Renderable.h"
#include "Light.h"
#include "Camera.h"

class Scene 
{
public:
	Scene(const Camera& camera);

	void addLight(const LightPtr& light);
	void addNode(const NodePtr& node);

	void render(GLFWwindow* window, float dt);
private:
	Node m_root;
	std::vector<LightPtr> m_lights;

	Camera m_camera;
};