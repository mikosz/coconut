#ifndef _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_
#define _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_

#include <cstdint>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

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

CCN_MAKE_POINTER_DEFINITIONS(DrawCommand);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_ */
