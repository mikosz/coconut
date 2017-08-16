#ifndef _COCONUT_PULP_WORLD_WORLD_HPP_
#define _COCONUT_PULP_WORLD_WORLD_HPP_

#include <chrono>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/fs.hpp"
#include "coconut/pulp/renderer/shader/Property.hpp"
#include "coconut/pulp/renderer/Scene.hpp"
#include "coconut/pulp/renderer/ModelFactory.hpp"
#include "coconut/pulp/renderer/Actor.hpp"
#include "Terrain.hpp"
#include "Sky.hpp"
#include "Windmap.hpp"

namespace coconut {
namespace pulp {
namespace world {

class World {
public:

	World(
		milk::graphics::Renderer& graphicsRenderer,
		renderer::Scene& scene, // TODO: don't register renderables in scene permanently?
		renderer::shader::PassFactory& passFactory, // TODO: inconvenient
		renderer::ModelFactory& modelFactory, // ,,
		const milk::FilesystemContext& fs
		) :
		windmap_(graphicsRenderer),
		terrain_(graphicsRenderer, scene, passFactory, modelFactory, fs),
		sky_(graphicsRenderer, scene, passFactory, fs)
	{
	}

	void update(std::chrono::milliseconds dt) {
		windmap_.update(dt);
	}

	// TODO: temp?
	void bindShaderProperties(renderer::shader::Properties& properties) const {
		// TODO: shouldn't create windmap each call
		properties.bind("windmap", renderer::shader::makeReflectiveObject(windmap_));
		terrain_.bindShaderProperties(properties, "terrain");
	}

	const Terrain& terrain() const noexcept {
		return terrain_;
	}

private:

	Windmap windmap_;
	
	Terrain terrain_;

	Sky sky_;

};

} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_WORLD_HPP_ */
