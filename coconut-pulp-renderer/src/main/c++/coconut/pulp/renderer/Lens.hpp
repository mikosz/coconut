#ifndef _COCONUT_PULP_RENDERER_LENS_HPP_
#define _COCONUT_PULP_RENDERER_LENS_HPP_

#include "coconut/pulp/math/Matrix.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Lens {
public:

	virtual const pulp::math::Matrix& projectionTransformation() const = 0;

protected:

	virtual ~Lens() {
	}

};

CCN_MAKE_POINTER_DEFINITIONS(Lens);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_LENS_HPP_ */
