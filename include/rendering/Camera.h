#pragma once

#include <iostream>

#include <glad/glad.h>
#include "glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "ShaderClass.h"
#include "Movable.h"

namespace vx 
{

class Camera : public Movable 
{
public:
	

	Camera(int width, int height, glm::vec3 position);
		
	const float& width() const;
	float& width();

	const float& height() const;
	float& height();

	void updateMatrix(float fov, float near_plane, float far_plane);

	const glm::mat4& viewMatrix() const;
	const glm::vec3& position() const;

	void handleInputs(GLFWwindow* window, float dt);

private:
	static constexpr glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::quat m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	float m_width, m_height;
	
	glm::mat4 m_camera_matrix = glm::mat4(1.0f);

	bool m_first_click = true;

	float m_speed = 1.0f;
	float m_sensitivity = 10000.0f;
};

}