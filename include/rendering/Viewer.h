#pragma once

#include <memory>
#include <vector>

#include <glad/glad.h>
#include "glfw3.h"
#include "Scene.h"

namespace vx 
{

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Viewer
{
public:
	Viewer(Viewer& viewer) = delete;
	Viewer& operator=(Viewer& viewer) = delete;

	static void Initialize(int width, int height);
	static void resize(int width, int height);

	static void addNode(const vx::NodePtr& node);
	static void run();

private:
	Viewer() = delete;
	~Viewer() = delete;

	static GLFWwindow* m_window;
	static Scene* m_scene;
};

}