#ifndef _COCONUT_PULP_RENDERER_MODEL_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_HPP_

#include <memory>

#include "milk/graphics/Buffer.hpp"
#include "milk/graphics/Device.hpp"
#include "milk/graphics/VertexShader.hpp"
#include "milk/graphics/PixelShader.hpp"
#include "Renderable.hpp"
#include "Oriented.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Model : public Renderable, public Oriented {
public:

	Model(
		milk::graphics::Device& device,
		std::shared_ptr<milk::graphics::VertexShader> vertexShader,
		std::shared_ptr<milk::graphics::PixelShader> pixelShader
		);

	void render(milk::graphics::Device& device) override;

private:

	milk::graphics::Buffer vertexBuffer_;

	milk::graphics::Buffer indexBuffer_;

	std::shared_ptr<milk::graphics::VertexShader> vertexShader_;
	
	std::shared_ptr<milk::graphics::PixelShader> pixelShader_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_HPP_ */
