#ifndef _COCONUT_PULP_WORLD_WORLD_HPP_
#define _COCONUT_PULP_WORLD_WORLD_HPP_

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/fs.hpp"
#include "coconut/pulp/renderer/Scene.hpp"
#include "coconut/pulp/renderer/ModelFactory.hpp"
#include "coconut/pulp/renderer/Actor.hpp"
#include "Terrain.hpp"
#include "Sky.hpp"

namespace coconut {
namespace pulp {
namespace world {

class World {
public:

	World(
		milk::graphics::Renderer& graphicsRenderer,
		renderer::Scene& scene,
		renderer::shader::PassFactory& passFactory, // TODO: inconvenient
		renderer::ModelFactory& modelFactory, // ,,
		const milk::FilesystemContext& fs
		) :
		terrain_(graphicsRenderer, scene, passFactory, modelFactory, fs),
		sky_(graphicsRenderer, scene, passFactory, fs)
	{
	}

private:

	Terrain terrain_;

	Sky sky_;

};

} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_WORLD_HPP_ */
