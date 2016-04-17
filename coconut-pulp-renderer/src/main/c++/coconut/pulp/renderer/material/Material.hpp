#ifndef _COCONUT_PULP_RENDERER_MATERIAL_MATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MATERIAL_MATERIAL_HPP_

#include <string>

#include <coconut-tools/exceptions/RuntimeError.hpp>
#include <coconut-tools/system/function-name.hpp>

#include "coconut/milk/math/Vector.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"
#include "coconut/milk/graphics/Texture2d.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace material {

class Material {
public:

	class NotImplemented : public coconut_tools::exceptions::RuntimeError {
	public:

		NotImplemented(const std::string& functionName) :
			coconut_tools::exceptions::RuntimeError("Function not implemented: " + functionName),
			functionName_(functionName)
		{
		}

		const std::string& functionName() const {
			return functionName_;
		}

	private:

		std::string functionName_;

	};

	virtual ~Material() {
	}

	virtual void setAmbientColour(const milk::math::Vector4d& rgbaColour) {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

	virtual const milk::math::Vector4d& ambientColour() const {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

	virtual void setDiffuseColour(const milk::math::Vector4d& rgbaColour) {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

	virtual const milk::math::Vector4d& diffuseColour() const {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

	virtual void setSpecularColour(const milk::math::Vector4d& rgbaColour) {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

	virtual const milk::math::Vector4d& specularColour() const {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

	virtual void setSpecularExponent(float specularExponent) {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

	virtual float specularExponent() const {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

	virtual void setDiffuseMap(milk::graphics::Texture2dUniquePtr diffuseMap) {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

	virtual milk::graphics::Texture2d& diffuseMap() const {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

};

CCN_MAKE_POINTER_DEFINITIONS(Material);

} // namespace material
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_MATERIAL_HPP_ */
