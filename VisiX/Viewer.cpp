#include "Viewer.h"

Viewer::Viewer(int width, int height) : m_scene(Camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)))
{
	// GLFW setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use modern OpenGL

	// Creation of a window of size widthxheight pixels
	m_window = glfwCreateWindow(width, height, "VisiX", NULL, NULL);
	if (m_window == NULL) {
		glfwTerminate();
		throw std::runtime_error("Window creation has failed.");
	}
	glfwMakeContextCurrent(m_window); // bind the window to be displayed

	gladLoadGL(); // Load glad to setup OpenGL context

	glViewport(0, 0, width, height); // Make the render viewport fill the window

	m_scene.addNode(m_renderable_node);
}

Viewer::~Viewer()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Viewer::addRenderable(const RenderablePtr& renderable)
{
	m_renderable_node->addRenderable(renderable);
}

void Viewer::addDynamicSystem(const DynamicSystemPtr& dynamic_system)
{
	m_dynamic_systems.emplace_back(dynamic_system);
}

void Viewer::run()
{
	double previous_time = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	// Main loop
	while (!glfwWindowShouldClose(m_window)) {
		glClearColor(0.07, 0.13, 0.17, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the back buffer by assigning the ClearColor 

		// Time evaluation
		double current_time = glfwGetTime();
		if (current_time - previous_time >= 1.0f / 60.0f) {
			previous_time = current_time;
		}

		for (auto& dynamic_system : m_dynamic_systems)
			dynamic_system->step();

		m_scene.render(m_window, current_time - previous_time);

		glfwSwapBuffers(m_window); // Don't forget to swap buffers to see changes

		glfwPollEvents(); // Receive potential events
	}
}
