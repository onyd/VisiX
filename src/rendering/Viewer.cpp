#include "Viewer.h"
#include "Camera.h"

namespace vx 
{

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Viewer::resize(width, height);
}

GLFWwindow* Viewer::m_window = nullptr;
Scene* Viewer::m_scene = nullptr;

void Viewer::Initialize(int width, int height)
{
	if (m_window != nullptr)
		throw std::runtime_error("Viewer has already been initialized.");

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
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	m_scene = new Scene(vx::Camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)));
}

void Viewer::resize(int width, int height)
{
	m_scene->resize(width, height);
}

void Viewer::addNode(const vx::NodePtr& node)
{
	m_scene->addNode(node);
}

void Viewer::run()
{
	double previous_time = glfwGetTime();

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// Main loop
	while (!glfwWindowShouldClose(m_window)) {
		glClearColor(0.07, 0.13, 0.17, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the back buffer by assigning the ClearColor 

		// Time evaluation
		double current_time = glfwGetTime();
		if (current_time - previous_time >= 1.0f / 60.0f) {
			previous_time = current_time;
		}

		m_scene->render(m_window, current_time - previous_time);

		glfwSwapBuffers(m_window); // Don't forget to swap buffers to see changes

		glfwPollEvents(); // Receive potential events
	}

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

}