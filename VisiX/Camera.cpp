#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 pos)
{
	m_width = width;
	m_height = height;
	m_pos = pos;
}

void Camera::updateMatrix(float fov, float near_plane, float far_plane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Compute the projection and view matrix
	view = glm::lookAt(m_pos, m_pos + m_forward, m_world_up);
	projection = glm::perspective(glm::radians(fov), (float)(m_width / m_height), near_plane, far_plane);

	// Update camera axis
	m_right = glm::vec3(view[0][0], view[1][0], view[2][0]);
	m_up = glm::vec3(view[0][1], view[1][1], view[2][1]);

	m_camera_matrix = projection * view;
}

void Camera::exportMatrix(const Shader& shader, const char* uniform) const
{
	glUniformMatrix4fv(glGetUniformLocation(shader.m_id, uniform), 1, GL_FALSE, glm::value_ptr(m_camera_matrix));
}

void Camera::exportPosition(const Shader& shader, const char* uniform) const
{
	glUniform3fv(glGetUniformLocation(shader.m_id, uniform), 1, glm::value_ptr(m_pos));
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
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
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

		// Compute x rotation
		glm::vec3 new_forward = glm::rotate(m_forward, glm::radians(-rotX), m_right);

		// Avoid gimbal lock issue by preventing m_forward from being vertical
		if ((abs(glm::angle(new_forward, m_world_up) - glm::radians(90.0f)) <= glm::radians(85.0f))) {
			m_forward = new_forward;
		}

		// Apply y rotation
		m_forward = glm::rotate(m_forward, glm::radians(-rotY), m_world_up);

		// Re-center cursor
		glfwSetCursorPos(window, (m_width / 2), (m_height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_first_click = true;
	}

}
