#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Renderable.h"
#include "Camera.h"
#include "Scene.h"
#include "Texture.h"
#include "Attribute.h"

#include "Viewer.h"

const int width = 1600;
const int height = 1600;

int main() {
	Viewer viewer(width, height);

	std::shared_ptr<SolidDynamicSystem> dynamic_system = std::make_shared<SolidDynamicSystem>(0.0001);
	viewer.addDynamicSystem(dynamic_system);

	Rectangle r1(glm::vec2(0.0, 0.0), 1.0, 1.0, 0.0, glm::vec3(1.0, 1.0, 1.0));
	r1.render(viewer);
	dynamic_system->registerSolid(std::make_shared<Rectangle>(r1));

	viewer.run();

	return 0;
}
