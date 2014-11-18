#ifndef _COCONUT_MILK_GRAPHICS_RENDERABLE_HPP_
#define _COCONUT_MILK_GRAPHICS_RENDERABLE_HPP_

#include "milk/math/Matrix.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class Renderable {
public:

	virtual ~Renderable() {
	}

	virtual void render(Device& device) = 0;

	virtual const math::Matrix& worldTransformation() const = 0;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_RENDERABLE_HPP_ */
