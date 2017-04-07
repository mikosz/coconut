#ifndef _COCONUT_PULP_RENDERER_RENDERER_HPP_
#define _COCONUT_PULP_RENDERER_RENDERER_HPP_

#include <vector>

#include "coconut/milk/graphics/CommandList.hpp"

#include "CommandBuffer.hpp"
#include "Scene.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Renderer {
public:

	void addScene(SceneSharedPtr scene);

	void render(milk::graphics::Renderer& renderer, milk::graphics::CommandList& commandList);

private:

	CommandBuffer commandBuffer_;

	std::vector<SceneSharedPtr> scenes_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_RENDERER_HPP_ */
