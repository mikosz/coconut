#ifndef _COCONUT_PULP_RENDERER_CAMERA_HPP_
#define _COCONUT_PULP_RENDERER_CAMERA_HPP_

#include "milk/math/Matrix.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Camera {
public:

	virtual const milk::math::Matrix& viewTransformation() const = 0;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_CAMERA_HPP_ */
