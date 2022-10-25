#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "ShaderClass.h"
#include "Node.h"

class Camera : public Movable 
{
	public:
		glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::mat4 m_camera_matrix = glm::mat4(1.0f);

		glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

		bool m_first_click = true;

		int m_width, m_height;

		float m_speed = 1.0f;
		float m_sensitivity = 10000.0f;

		/**
		 * @fn	Camera::Camera(int width, int height, glm::vec3 position);
		 *
		 * @brief	Constructor
		 *
		 * @param 	width   	The width.
		 * @param 	height  	The height.
		 * @param 	position	The position.
		 */
		Camera(int width, int height, glm::vec3 position);

		/**
		 * @fn	void Camera::updateMatrix(float fov, float nearPlane, float farPlane);
		 *
		 * @brief	Updates the projection view matrix
		 *
		 * @param 	fov		  	The fov.
		 * @param 	nearPlane	The near plane.
		 * @param 	farPlane 	The far plane.
		 */
		void updateMatrix(float fov, float near_plane, float far_plane);

		/**
		 * @fn	void Camera::exportMatrix(Shader& shader, const char* uniform) const;
		 *
		 * @brief	Export matrix as uniform int the shader program
		 *
		 * @param [in]	shader 	The shader.
		 * @param 		  	uniform	The uniform.
		 */
		void exportMatrix(const Shader& shader, const char* uniform) const;

		/**
		 * @fn	void Camera::exportPosition(Shader& shader, const char* uniform);
		 *
		 * @brief	Export position as uniform in the shader program
		 *
		 * @param [in]	shader 	The shader.
		 * @param 		  	uniform	The uniform.
		 */
		void exportPosition(const Shader& shader, const char* uniform) const;

		/**
		 * @fn	void Camera::handleInputs(GLFWwindow* window, float dt) const;
		 *
		 * @brief	Handles the inputs for camera control
		 *
		 * @param [in]	window	If non-null, the window.
		 * @param 		  	dt	  	The dt.
		 */
		void handleInputs(GLFWwindow* window, float dt);
};