#ifndef _COCONUT_PULP_RENDERER_DRAW_COMMAND_HPP_
#define _COCONUT_PULP_RENDERER_DRAW_COMMAND_HPP_

#include <cstdint>

namespace coconut {
namespace pulp {
namespace renderer {

class DrawCommand {
public:

	using Key = std::uint64_t;

	DrawCommand(const DrawCommand&) = delete;

	void operator=(const DrawCommand&) = delete;

	virtual ~DrawCommand() = default;

	virtual Key key() const = 0;

	virtual void run(milk::graphics::Device& graphicsDevice) = 0;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_DRAW_COMMAND_HPP_ */
