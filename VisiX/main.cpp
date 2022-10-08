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
//// Vertices coordinates
//Vertex vertices[] =
//{ //               COORDINATES           /            COLORS          /           TexCoord         /       NORMALS         //
//	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
//	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
//	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
//	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
//};
//
//// Indices for vertices order
//GLuint indices[] =
//{
//	0, 1, 2,
//	0, 2, 3
//};
//
//Vertex light_verticies[] =
//{ //     COORDINATES     //
//	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
//	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
//	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
//	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
//	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
//	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
//	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
//	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
//};
//
//GLuint light_indicies[] =
//{
//	0, 1, 2,
//	0, 2, 3,
//	0, 4, 7,
//	0, 7, 3,
//	3, 7, 6,
//	3, 6, 2,
//	2, 6, 5,
//	2, 5, 1,
//	1, 5, 4,
//	1, 4, 0,
//	4, 5, 6,
//	4, 6, 7
//};
//
//
//const int width = 800;
//const int height = 800;
//
//int main() {
//	// GLFW setup
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
//	// Texture data
//	Texture textures[]
//	{
//		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_RGBA),
//		Texture("planksSpec.png", "specular", 1, GL_RED, GL_RED)
//	};
//
//	glViewport(0, 0, width, height); // Make the render viewport fill the window
//	Shader shader_program = Shader::from_file("default.vert", "default.frag");
//
//	std::vector<Vertex> verticies(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
//	std::vector<GLuint> indicies(indices, indices + sizeof(indices) / sizeof(GLuint));
//	std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
//	// Create floor Mesh
//	Mesh floor(verticies, indicies, tex);
//	
//	// Shader for light cube
//	Shader light_shader = Shader::from_file("light.vert", "light.frag");
//	// Store Mesh data in vectors for the Mesh
//	std::vector<Vertex> light_verts(light_verticies, light_verticies + sizeof(light_verticies) / sizeof(Vertex));
//	std::vector<GLuint> light_inds(light_indicies, light_indicies + sizeof(light_indicies) / sizeof(GLuint));
//	// Crate light Mesh
//	Mesh light(light_verts, light_inds, tex);
//
//
//	glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//	glm::vec3 light_pos = glm::vec3(0.5f, 0.5f, 0.5f);
//	glm::mat4 light_model = glm::mat4(1.0f);
//	light_model = glm::translate(light_model, light_pos);
//
//	glm::vec3 object_pos = glm::vec3(0.0f, 0.0f, 0.0f);
//	glm::mat4 object_model = glm::mat4(1.0f);
//	object_model = glm::translate(object_model, object_pos);
//
//	MeshRenderable ligth_renderable(light, 
//									light_shader, 
//									{ { light_shader, std::make_shared<Mat4Attribute>("model", light_model) }, 
//									  { light_shader, std::make_shared<Vec4Attribute>("lightColor", light_color) } });
//
//	MeshRenderable floor_renderable( floor, 
//									shader_program, 
//									{ { shader_program, std::make_shared<Mat4Attribute>("model", object_model) },
//									  { shader_program, std::make_shared<Vec4Attribute>("lightColor", light_color) },
//									  { shader_program, std::make_shared<Vec3Attribute>("lightPos", light_pos) } });
//	
//	// Camera
//	Camera cam(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
//
//	double previous_time = glfwGetTime();
//
//	glEnable(GL_DEPTH_TEST);
//
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
//		cam.updateMatrix(45.0f, 0.1f, 100.0f); // Update the new projection view matrix
//
//		floor_renderable.draw(cam); 
//		ligth_renderable.draw(cam);
//
//
//		glfwSwapBuffers(window); // Don't forget to swap buffers to see changes
//
//		glfwPollEvents(); // Receive potential events
//	}
//
//	// Destroy objects before exiting
//	shader_program.destroy();
//	light_shader.destroy();
//
//	glfwDestroyWindow(window);
//	glfwTerminate();
//	return 0;
//}

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Renderable.h"
#include "Camera.h"

const int width = 1500;
const int height = 1500;

int main() {
	// GLFW setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use modern OpenGL

	// Creation of a window of size widthxheight pixels
	GLFWwindow* window = glfwCreateWindow(width, height, "VisiX", NULL, NULL);
	if (window == NULL) {
		std::cout << "Window creation has failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // bind the window to be displayed

	gladLoadGL(); // Load glad to setup OpenGL context

	glViewport(0, 0, width, height); // Make the render viewport fill the window
	Shader shader_program = Shader::from_file("particle.vert", "particle.frag");

	std::vector<GLfloat> positions;
	std::vector<GLfloat> velocities;

	int N = 2048;
	for (int i = 0; i < N; i++) {
		positions.emplace_back(((float)rand() / RAND_MAX - 0.5) * 2);
		positions.emplace_back(((float)rand() / RAND_MAX - 0.5) * 2);
		positions.emplace_back(((float)rand() / RAND_MAX - 0.5) * 2);
		positions.emplace_back(1.0);
		velocities.emplace_back(0.0);
		velocities.emplace_back(0.0);
		velocities.emplace_back(0.0);
		velocities.emplace_back(0.0);
	}

	VAO vao;
	vao.bind();
	Buffer pos_buffer(positions, GL_DYNAMIC_DRAW);
	vao.linkAttribute(pos_buffer, 0, 4, GL_FLOAT, 4 * sizeof(GLfloat), (void*) 0);
	vao.unbind();

	Buffer vel_buffer(velocities, GL_DYNAMIC_DRAW);

	ComputeShader compute_shader = ComputeShader::from_file("particle.comp", { pos_buffer, vel_buffer });

	glm::mat4 model = glm::mat4(1.0f);
	Mat4Attribute model_matrix("model", model);
	model_matrix.exportAsUniform(shader_program);

	glm::vec4 particle_color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	Vec4Attribute particle_color_attr("particleColor", particle_color);
	particle_color_attr.exportAsUniform(shader_program);

	// Camera
	Camera cam(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	double previous_time = glfwGetTime();
	glEnable(GL_PROGRAM_POINT_SIZE);
	// Main loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07, 0.13, 0.17, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the back buffer by assigning the ClearColor 

		// Time evaluation
		double current_time = glfwGetTime();
		if (current_time - previous_time >= 1.0f / 60.0f) {
			previous_time = current_time;
		}

		// Camera update
		cam.handleInputs(window, current_time - previous_time); // Handle camera controls (move, rotation, ...)
		cam.updateMatrix(45.0f, 0.1f, 100.0f); // Update the new projection view matrix

		compute_shader.activate();
		compute_shader.dispatch(N / 32, 1, 1);

		shader_program.activate();
		cam.exportMatrix(shader_program, "cameraMatrix");

		vao.bind();
		vao.linkAttribute(pos_buffer, 0, 4, GL_FLOAT, 4 * sizeof(GLfloat), 0);
		glDrawArrays(GL_POINTS, 0, N);
		vao.unbind();

		glfwSwapBuffers(window); // Don't forget to swap buffers to see changes

		glfwPollEvents(); // Receive potential events
	}

	// Destroy objects before exiting
	shader_program.destroy();
	compute_shader.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}