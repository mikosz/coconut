#ifndef _COCONUT_PULP_WORLD_SKY_HPP_
#define _COCONUT_PULP_WORLD_SKY_HPP_

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/fs.hpp"
#include "coconut/pulp/renderer/Scene.hpp"
#include "coconut/pulp/renderer/ModelFactory.hpp"
#include "coconut/pulp/renderer/Actor.hpp"

namespace coconut {
namespace pulp {
namespace world {

class Sky {
public:

	Sky(
		milk::graphics::Renderer& graphicsRenderer,
		renderer::Scene& scene,
		renderer::shader::PassFactory& passFactory, // TODO: inconvenient
		const milk::FilesystemContext& fs
		);

private:



};

} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_SKY_HPP_ */
