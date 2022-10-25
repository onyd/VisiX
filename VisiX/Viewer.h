#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "DynamicSystem.h"

class Viewer
{
public:
	Viewer(int width, int height);
	~Viewer();

	void addRenderable(const RenderablePtr& renderable);
	void addDynamicSystem(const DynamicSystemPtr& dynamic_system);

	void run();

private:
	GLFWwindow* m_window;
	Scene m_scene;
	std::shared_ptr<RenderableNode> m_renderable_node = std::make_shared<RenderableNode>();
	std::vector<std::shared_ptr<DynamicSystemBase>> m_dynamic_systems;
};