
//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "Mesh.h"
//#include "Renderable.h"
//#include "Camera.h"
//
//const int width = 1500;
//const int height = 1500;
//
//int main() {
//	// GLFW setup
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use modern OpenGL
//
//	// Creation of a window of size widthxheight pixels
//	GLFWwindow* window = glfwCreateWindow(width, height, "VisiX", NULL, NULL);
//	if (window == NULL) {
//		std::cout << "Window creation has failed" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window); // bind the window to be displayed
//
//	gladLoadGL(); // Load glad to setup OpenGL context
//
//	glViewport(0, 0, width, height); // Make the render viewport fill the window
//	Shader shader_program = Shader::from_file("particle.vert", "particle.frag");
//
//	std::vector<GLfloat> positions;
//	std::vector<GLfloat> velocities;
//
//	int N = 4096*8;
//	for (int i = 0; i < N; i++) {
//		positions.emplace_back(((float)rand() / RAND_MAX - 0.5) * 2);
//		positions.emplace_back(((float)rand() / RAND_MAX - 0.5) * 2);
//		positions.emplace_back(((float)rand() / RAND_MAX - 0.5) * 2);
//		positions.emplace_back(1.0);
//		velocities.emplace_back(0.0);
//		velocities.emplace_back(0.0);
//		velocities.emplace_back(0.0);
//		velocities.emplace_back(0.0);
//	}
//
//	VAO vao;
//	vao.bind();
//	Buffer pos_buffer(positions, GL_DYNAMIC_DRAW);
//	vao.linkAttribute(pos_buffer, 0, 4, GL_FLOAT, 4 * sizeof(GLfloat), (void*) 0);
//	vao.unbind();
//
//	Buffer vel_buffer(velocities, GL_DYNAMIC_DRAW);
//
//	ComputeShader compute_shader = ComputeShader::from_file("orbiting.comp", { pos_buffer });
//
//	glm::mat4 model = glm::mat4(1.0f);
//	Mat4Attribute model_matrix("model", model);
//	model_matrix.exportAsUniform(shader_program);
//
//	/*glm::vec4 particle_color = glm::vec4(1.0f, 0.0f, 1.0f, 0.5f);
//	Vec4Attribute particle_color_attr("particleColor", particle_color);
//	particle_color_attr.exportAsUniform(shader_program);*/
//
//	// Camera
//	Camera cam(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
//	double previous_time = glfwGetTime();
//	glEnable(GL_PROGRAM_POINT_SIZE);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//	// Main loop
//	while (!glfwWindowShouldClose(window)) {
//		glClearColor(0.07, 0.13, 0.17, 1.0);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the back buffer by assigning the ClearColor 
//
//		// Time evaluation
//		double current_time = glfwGetTime();
//		if (current_time - previous_time >= 1.0f / 60.0f) {
//			previous_time = current_time;
//		}
//
//		// Camera update
//		cam.handleInputs(window, current_time - previous_time); // Handle camera controls (move, rotation, ...)
//		cam.updateMatrix(45.0f, 0.1f, 1000.0f); // Update the new projection view matrix
//
//		FloatAttribute dt("dt", current_time - previous_time);
//		dt.exportAsUniform(compute_shader);
//
//		compute_shader.activate();
//		compute_shader.dispatch(N / 32, 1, 1);
//
//		shader_program.activate();
//		cam.exportMatrix(shader_program, "cameraMatrix");
//
//		vao.bind();
//		vao.linkAttribute(pos_buffer, 0, 4, GL_FLOAT, 4 * sizeof(GLfloat), 0);
//		glDrawArrays(GL_POINTS, 0, N);
//		vao.unbind();
//
//		glfwSwapBuffers(window); // Don't forget to swap buffers to see changes
//
//		glfwPollEvents(); // Receive potential events
//	}
//
//	// Destroy objects before exiting
//	shader_program.destroy();
//	compute_shader.destroy();
//
//	glfwDestroyWindow(window);
//	glfwTerminate();
//	return 0;
//}