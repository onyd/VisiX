
#include <iostream>

#include "Particles.h"
#include "Object.h"
#include "Viewer.h"

const int width = 1200;
const int height = 800;

struct Particle
{
	glm::vec4 pos;
	glm::vec4 vel;
};

int main() {
	vx::Viewer::Initialize(width, height);

	vx::Shader shader_program = vx::Shader::from_file("../resources/particle.vert", "../resources/particle.frag");

	std::vector<Particle> p;

	int N = 1 << 20;
	for (int i = 0; i < N; i++) {
		p.emplace_back(Particle{ glm::vec4(((float)rand() / RAND_MAX - 0.5) * 2, ((float)rand() / RAND_MAX - 0.5) * 2, ((float)rand() / RAND_MAX - 0.5) * 2, 1.0f), glm::vec4(0.0) });
	}

	vx::Particles particles(p, { vx::VertexFormat<4, float>::value, vx::VertexFormat<4, float>::value }, shader_program, "../resources/compute_shaders/three_scroll.comp");
	auto node = std::make_shared<vx::Object>(std::make_shared<vx::Particles>(particles));
	
	vx::Viewer::addNode(node);

	vx::Viewer::run();

	return 0;
}