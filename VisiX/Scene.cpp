#include "Scene.h"

Scene::Scene(const Camera& camera) : m_camera(camera)
{
}

void Scene::addLight(const LightPtr& light)
{
	m_lights.emplace_back(light);
}

void Scene::addNode(const NodePtr& node)
{
	m_root.addNode(node);
}

void Scene::render(GLFWwindow* window, float dt)
{
	m_camera.handleInputs(window, dt); // Handle camera controls (move, rotation, ...)
	m_camera.updateMatrix(45.0f, 0.1f, 100.0f); // Update the new projection view matrix
	m_root.draw(m_camera);
}
