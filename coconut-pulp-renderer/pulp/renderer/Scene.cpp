#include "Scene.hpp"

#include "Lens.hpp"
#include "Model.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

void Scene::render(milk::graphics::Device& device, Model& model, const Lens& lens) {
	model.render(device);
}
