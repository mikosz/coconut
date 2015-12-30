#ifndef _COCONUT_PULP_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_
#define _COCONUT_PULP_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace lighting {

class DirectionalLight {
public:

	DirectionalLight(
		const milk::math::Vector3d& direction,
		const milk::math::Vector4d& ambientColour,
		const milk::math::Vector4d& diffuseColour,
		const milk::math::Vector4d& specularColour
		) :
		direction_(direction),
		ambientColour_(ambientColour),
		diffuseColour_(diffuseColour),
		specularColour_(specularColour)
	{
	}

	const milk::math::Vector3d& direction() const {
		return direction_;
	}

	const milk::math::Vector4d& ambientColour() const {
		return ambientColour_;
	}

	const milk::math::Vector4d& diffuseColour() const {
		return diffuseColour_;
	}

	const milk::math::Vector4d& specularColour() const {
		return specularColour_;
	}

private:

	milk::math::Vector3d direction_;

	milk::math::Vector4d ambientColour_;

	milk::math::Vector4d diffuseColour_;

	milk::math::Vector4d specularColour_;

};

} // namespace lighting
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_ */
