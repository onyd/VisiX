#pragma once

#include <vector>

#include "Node.h"
#include "Light.h"

class Scene {
	public:
		Node* root;
		std::vector<Light*> lights;

		void render();
};