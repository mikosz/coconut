#ifndef _COCONUT_PULP_RENDERER_GEOMETRY_DRAW_COMMAND_HPP_
#define _COCONUT_PULP_RENDERER_GEOMETRY_DRAW_COMMAND_HPP_

#include "coconut/milk/graphics/Device.hpp"
#include "coconut/milk/graphics/VertexShader.hpp"
#include "coconut/milk/graphics/PixelShader.hpp"

#include "DrawCommand.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class GeometryDrawCommand : public DrawCommand {
public:

	Key key() const override;

	void run(milk::graphics::Device& graphicsDevice) override;

private:

	milk::graphics::VertexShader* vertexShader_;

	milk::graphics::PixelShader* pixelShader_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_GEOMETRY_DRAW_COMMAND_HPP_ */
