#ifndef _COCONUT_PULP_RENDERER_MATERIAL_PHONGMATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MATERIAL_PHONGMATERIAL_HPP_

#include "Material.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace material {

class PhongMaterial : public Material {
public:

	void setDiffuseColour(const milk::math::Vector4d& rgbaColour) override {
		diffuseColour_ = rgbaColour;
	}

	const milk::math::Vector4d& diffuseColour() const override {
		return diffuseColour_;
	}

private:

	milk::math::Vector4d diffuseColour_;

};

} // namespace material
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_PHONGMATERIAL_HPP_ */

