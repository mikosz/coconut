#ifndef _COCONUT_PULP_RENDERER_LIGHTING_POINTLIGHT_HPP_
#define _COCONUT_PULP_RENDERER_LIGHTING_POINTLIGHT_HPP_

#include "coconut/pulp/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace lighting {

class PointLight {
public:

	PointLight(
		const pulp::math::Vec3& position,
		const pulp::math::Vec3& attenuation, // <constant, linear, quadratic>
		const pulp::math::Vec4& ambientColour,
		const pulp::math::Vec4& diffuseColour,
		const pulp::math::Vec4& specularColour
		) :
		position_(position),
		attenuation_(attenuation),
		ambientColour_(ambientColour),
		diffuseColour_(diffuseColour),
		specularColour_(specularColour)
	{
	}

	const pulp::math::Vec3& position() const {
		return position_;
	}

	const pulp::math::Vec3& attenuation() const {
		return attenuation_;
	}

	const pulp::math::Vec4& ambientColour() const {
		return ambientColour_;
	}

	const pulp::math::Vec4& diffuseColour() const {
		return diffuseColour_;
	}

	const pulp::math::Vec4& specularColour() const {
		return specularColour_;
	}

private:

	pulp::math::Vec3 position_;

	pulp::math::Vec3 attenuation_;

	pulp::math::Vec4 ambientColour_;

	pulp::math::Vec4 diffuseColour_;

	pulp::math::Vec4 specularColour_;

};

} // namespace lighting
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_LIGHTING_POINTLIGHT_HPP_ */
