#include "Camera.h"

namespace vx 
{

Camera::Camera(int width, int height, glm::vec3 pos)
{
	m_width = width;
	m_height = height;
	m_pos = pos;
}

const float& Camera::width() const
{
	return m_width;
}

float& Camera::width()
{
	return m_width;
}

const float& Camera::height() const
{
	return m_height;
}

float& Camera::height()
{
	return m_height;

}

void Camera::updateMatrix(float fov, float near_plane, float far_plane)
{
	// Compute the projection and view matrix
	glm::mat4 view = glm::toMat4(m_orientation);
	view[3] = glm::vec4(-(m_orientation * m_pos), 1.0f);

	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)(m_width / m_height), near_plane, far_plane);

	// Update camera basis
	m_right = glm::vec3(view[0][0], view[1][0], view[2][0]);
	m_up = glm::vec3(view[0][1], view[1][1], view[2][1]);
	m_forward = -glm::vec3(view[0][2], view[1][2], view[2][2]);

	m_camera_matrix = projection * view;
}

const glm::mat4& Camera::viewMatrix() const
{
	return m_camera_matrix;
}

const glm::vec3& Camera::position() const
{
	return m_pos;
}

void Camera::handleInputs(GLFWwindow* window, float dt)
{
	// ZQSD control
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		move(dt * m_speed * m_forward);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		move(-dt * m_speed * m_right);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		move(-dt * m_speed * m_forward);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		move(dt * m_speed * m_right);

	// Vertical control
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		move(dt * m_speed * m_world_up);
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		move(-dt * m_speed * m_world_up);

	// m_speed control
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		m_speed = 2.0f;
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		m_speed = 1.0f;

	// Mouse control
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		// Hide the cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Avoid jump on first click
		if (m_first_click) {
			glfwSetCursorPos(window, (m_width / 2), (m_height / 2));
			m_first_click = false;
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = dt * m_sensitivity * (float)(mouseY - (m_height / 2)) / m_height;
		float rotY = dt * m_sensitivity * (float)(mouseX - (m_width / 2)) / m_width;

		m_orientation = glm::rotate(m_orientation, glm::radians(rotX), m_right);
		m_orientation = glm::rotate(m_orientation, glm::radians(rotY), m_world_up);

		// Re-center cursor
		glfwSetCursorPos(window, (m_width / 2), (m_height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_first_click = true;
	}

}

}