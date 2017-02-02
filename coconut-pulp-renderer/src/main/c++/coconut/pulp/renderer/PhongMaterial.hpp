#ifndef _COCONUT_PULP_RENDERER_PHONGMATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_PHONGMATERIAL_HPP_

#include "coconut/milk/math/Vector.hpp"

#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/Sampler.hpp"

#include "Material.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

#pragma message("material fix")
/* replace with something like this:
class PhongMaterial : public Material {
public:

	bool getBooleanAttribute(AttributeId attributeId) const override {
		switch (attributeId) {
		case AttributeId::HAS_DIFFUSE_MAP:
			return hasDiffuseMap_;
		default:
			throw UnhandledMaterialAttribute(attributeId);
		}
	}

	...

	Pass::Id getShaderPassId() const override {
		return shaderPassId_;
	}

};
*/

class PhongMaterial : public Material {
public:

	static const std::string TYPE;

	PhongMaterial(ShaderPassType shaderPassType) :
		Material(shaderPassType)
	{
	}

	const std::string& type() const override {
		return TYPE;
	}

	void setAmbientColour(const milk::math::Vector4d& rgbaColour) { // TODO: colour type
		ambientColour_ = rgbaColour;
	}

	const milk::math::Vector4d& ambientColour() const {
		return ambientColour_;
	}

	void setDiffuseColour(const milk::math::Vector4d& rgbaColour) {
		diffuseColour_ = rgbaColour;
	}

	const milk::math::Vector4d& diffuseColour() const {
		return diffuseColour_;
	}

	void setSpecularColour(const milk::math::Vector4d& rgbaColour) {
		specularColour_ = rgbaColour;
	}

	const milk::math::Vector4d& specularColour() const {
		return specularColour_;
	}

	void setSpecularExponent(float specularExponent) {
		specularExponent_ = specularExponent;
	}

	float specularExponent() const {
		return specularExponent_;
	}

	bool hasDiffuseMap() const {
		return diffuseMap_ != nullptr;
	}

	void setDiffuseMap(milk::graphics::Texture2dUniquePtr diffuseMap) {
		diffuseMap_ = std::move(diffuseMap);
	}

	milk::graphics::Texture2d& diffuseMap() const {
		// TODO: const returns reference to non-const
#pragma message("fixme")
		assert(diffuseMap_);
		return *diffuseMap_;
	}

	void setDiffuseMapSampler(milk::graphics::Sampler sampler) {
		diffuseMapSampler_ = std::move(sampler);
	}

	milk::graphics::Sampler& diffuseMapSampler() const {
		// TODO: const returns reference to non-const
#pragma message("fixme!!!!!!!!!")
		return const_cast<milk::graphics::Sampler&>(diffuseMapSampler_);
	}

private:

	milk::math::Vector4d ambientColour_;

	milk::math::Vector4d diffuseColour_;

	milk::math::Vector4d specularColour_;

	float specularExponent_;

	milk::graphics::Texture2dUniquePtr diffuseMap_;

	milk::graphics::Sampler diffuseMapSampler_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_PHONGMATERIAL_HPP_ */
