#ifndef _COCONUT_PULP_RENDERER_CAMERA_HPP_
#define _COCONUT_PULP_RENDERER_CAMERA_HPP_

#include "coconut/milk/math/Matrix.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Camera {
public:

	virtual const milk::math::Matrix& viewTransformation() const = 0;

protected:

	virtual ~Camera() {
	}

};

MAKE_POINTER_DEFINITIONS(Camera);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_CAMERA_HPP_ */
