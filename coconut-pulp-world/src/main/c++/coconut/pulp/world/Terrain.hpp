#ifndef _COCONUT_PULP_WORLD_TERRAIN_HPP_
#define _COCONUT_PULP_WORLD_TERRAIN_HPP_

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/fs.hpp"
#include "coconut/pulp/renderer/shader/ReflectiveObject.hpp"
#include "coconut/pulp/renderer/Scene.hpp"
#include "coconut/pulp/renderer/ModelFactory.hpp"
#include "coconut/pulp/renderer/Actor.hpp"
#include "Heightmap.hpp"

namespace coconut {
namespace pulp {
namespace world {

class Terrain {
public:

	Terrain(
		milk::graphics::Renderer& graphicsRenderer,
		renderer::Scene& scene,
		renderer::shader::PassFactory& passFactory, // TODO: inconvenient
		renderer::ModelFactory& modelFactory, // ,,
		const milk::FilesystemContext& fs
		);

	void bindShaderProperties(
		renderer::shader::Properties& properties,
		std::string objectId
		) const;

private:

	Heightmap heightmap_;

	friend class renderer::shader::ReflectiveInterface<Terrain>;

};

} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_TERRAIN_HPP_ */
