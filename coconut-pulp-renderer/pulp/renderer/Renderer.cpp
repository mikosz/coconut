#include "Renderer.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

void Renderer::addScene(SceneSharedPtr scene) {
	scenes_.push_back(scene);
}

void Renderer::render(milk::graphics::Device& graphicsDevice) {
	graphicsDevice.beginScene();

	for (auto scene : scenes_) {
		scene->render(graphicsDevice);
	}

	graphicsDevice.endScene();
}
