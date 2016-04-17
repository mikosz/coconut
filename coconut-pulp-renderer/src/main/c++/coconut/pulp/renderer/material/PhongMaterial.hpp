#ifndef _COCONUT_PULP_RENDERER_MATERIAL_PHONGMATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MATERIAL_PHONGMATERIAL_HPP_

#include "Material.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace material {

class PhongMaterial : public Material {
public:

	void setAmbientColour(const milk::math::Vector4d& rgbaColour) override {
		ambientColour_ = rgbaColour;
	}

	const milk::math::Vector4d& ambientColour() const override {
		return ambientColour_;
	}

	void setDiffuseColour(const milk::math::Vector4d& rgbaColour) override {
		diffuseColour_ = rgbaColour;
	}

	const milk::math::Vector4d& diffuseColour() const override {
		return diffuseColour_;
	}

	void setSpecularColour(const milk::math::Vector4d& rgbaColour) override {
		specularColour_ = rgbaColour;
	}

	const milk::math::Vector4d& specularColour() const override {
		return specularColour_;
	}

	void setSpecularExponent(float specularExponent) override {
		specularExponent_ = specularExponent;
	}

	float specularExponent() const override {
		return specularExponent_;
	}

	void setDiffuseMap(milk::graphics::Texture2dUniquePtr diffuseMap) override {
		diffuseMap_ = std::move(diffuseMap);
	}

	milk::graphics::Texture2d& diffuseMap() const override {
		return *diffuseMap_;
	}

private:

	milk::math::Vector4d ambientColour_;

	milk::math::Vector4d diffuseColour_;

	milk::math::Vector4d specularColour_;

	float specularExponent_;

	milk::graphics::Texture2dUniquePtr diffuseMap_;

};

} // namespace material
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_PHONGMATERIAL_HPP_ */
