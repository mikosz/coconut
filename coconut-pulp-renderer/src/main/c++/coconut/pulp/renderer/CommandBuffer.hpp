#ifndef _COCONUT_PULP_RENDERER_COMMANDBUFFER_HPP_
#define _COCONUT_PULP_RENDERER_COMMANDBUFFER_HPP_

#include <vector>
#include <cstdint>

#include "DrawCommand.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class CommandBuffer {
public:

	void add(DrawCommandUniquePtr drawCommand);

private:

	using Commands = std::vector<DrawCommandSharedPtr>;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_COMMANDBUFFER_HPP_ */
