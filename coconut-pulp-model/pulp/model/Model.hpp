#ifndef _COCONUT_PULP_MODEL_HPP_
#define _COCONUT_PULP_MODEL_HPP_

#include <memory>

#include "milk/graphics/Buffer.hpp"
#include "milk/graphics/Device.hpp"
#include "milk/graphics/VertexShader.hpp"
#include "milk/graphics/PixelShader.hpp"

namespace coconut {
namespace pulp {
namespace model {

class Model {
public:

	Model(milk::graphics::Device& device, std::shared_ptr<milk::graphics::VertexShader> vertexShader, std::shared_ptr<milk::graphics::PixelShader> pixelShader);

	void render(milk::graphics::Device& device);

private:

	milk::graphics::Buffer vertexBuffer_;

	milk::graphics::Buffer indexBuffer_;

	std::shared_ptr<milk::graphics::VertexShader> vertexShader_;
	
	std::shared_ptr<milk::graphics::PixelShader> pixelShader_;

};

} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MODEL_HPP_ */
