#ifndef _COCONUT_PULP_RENDERER_LENS_HPP_
#define _COCONUT_PULP_RENDERER_LENS_HPP_

#include <memory>

#include "milk/math/Matrix.hpp"
#include "milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Lens {
public:

	virtual const milk::math::Matrix& projectionTransformation() const = 0;

};

MAKE_POINTER_DEFINITIONS(Lens);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_LENS_HPP_ */
