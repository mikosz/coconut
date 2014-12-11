#ifndef _COCONUT_PULP_RENDERER_RENDERABLE_HPP_
#define _COCONUT_PULP_RENDERER_RENDERABLE_HPP_

#include "milk/graphics/Device.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Renderable {
public:

	virtual ~Renderable() {
	}

	virtual void render(milk::graphics::Device& device) = 0;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_RENDERABLE_HPP_ */
