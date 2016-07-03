#ifndef _COCONUT_PULP_RENDERER_LIGHTING_POINTLIGHT_HPP_
#define _COCONUT_PULP_RENDERER_LIGHTING_POINTLIGHT_HPP_

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace lighting {

class PointLight {
public:

	PointLight(
		const milk::math::Vector3d& position,
		const milk::math::Vector3d& attenuation, // <constant, linear, quadratic>
		const milk::math::Vector4d& ambientColour,
		const milk::math::Vector4d& diffuseColour,
		const milk::math::Vector4d& specularColour
		) :
		position_(position),
		attenuation_(attenuation),
		ambientColour_(ambientColour),
		diffuseColour_(diffuseColour),
		specularColour_(specularColour)
	{
	}

	const milk::math::Vector3d& position() const {
		return position_;
	}

	const milk::math::Vector3d& attenuation() const {
		return attenuation_;
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

	milk::math::Vector3d position_;

	milk::math::Vector3d attenuation_;

	milk::math::Vector4d ambientColour_;

	milk::math::Vector4d diffuseColour_;

	milk::math::Vector4d specularColour_;

};

} // namespace lighting
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_LIGHTING_POINTLIGHT_HPP_ */
