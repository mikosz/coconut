#ifndef _COCONUT_PULP_WORLD_FOLIAGE_GRASS_HPP_
#define _COCONUT_PULP_WORLD_FOLIAGE_GRASS_HPP_

#include "coconut/milk/fs.hpp"

#include "coconut/pulp/renderer/shader/PassFactory.hpp"
#include "coconut/pulp/renderer/Actor.hpp"

namespace coconut {
namespace pulp {
namespace world {
namespace foliage {

class Grass : public renderer::Actor {
public:

	Grass(
		milk::graphics::Renderer& graphicsRenderer,
		renderer::shader::PassFactory& passFactory,
		milk::FilesystemContext& filesystemContext
		);

	void update(std::chrono::milliseconds dt) override;

	void render(renderer::CommandBuffer& commandBuffer, renderer::PassContext PassContext) override;

private:



};

} // namespace foliage
} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_FOLIAGE_GRASS_HPP_ */
