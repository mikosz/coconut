#ifndef _COCONUT_PULP_RENDERER_COMMANDBUFFER_HPP_
#define _COCONUT_PULP_RENDERER_COMMANDBUFFER_HPP_

#include <vector>
#include <cstdint>

#include "coconut/milk/graphics/CommandList.hpp"

#include "DrawCommand.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class CommandBuffer {
public:

	void add(DrawCommandUniquePtr drawCommand);

	void submit(milk::graphics::CommandList& commandList);

private:

	using Commands = std::vector<DrawCommandSharedPtr>;

	Commands commands_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_COMMANDBUFFER_HPP_ */
