#ifndef _COCONUT_PULP_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_
#define _COCONUT_PULP_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_

#include "coconut/pulp/math/Vector.hpp"
#include "coconut/pulp/renderer/shader/ReflectiveObject.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace lighting {

class DirectionalLight {
public:

	DirectionalLight(
		const pulp::math::Vec3& direction,
		const pulp::math::Vec4& ambientColour,
		const pulp::math::Vec4& diffuseColour,
		const pulp::math::Vec4& specularColour
		) :
		direction_(direction),
		ambientColour_(ambientColour),
		diffuseColour_(diffuseColour),
		specularColour_(specularColour)
	{
	}

	const pulp::math::Vec3& direction() const {
		return direction_;
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

	pulp::math::Vec3 direction_;

	pulp::math::Vec4 ambientColour_;

	pulp::math::Vec4 diffuseColour_;

	pulp::math::Vec4 specularColour_;

	friend class renderer::shader::ReflectiveInterface<DirectionalLight>;

};

} // namespace lighting

namespace shader {

template <> // TODO: can avoid declaring this in .hpp by creating a generic template with
// undefined default constructor and only defining it in .cpp
class ReflectiveInterface<lighting::DirectionalLight> :
	public ReflectiveInterfaceBase<lighting::DirectionalLight>
{
public:

	ReflectiveInterface();

};

} // namespace shader

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_ */
