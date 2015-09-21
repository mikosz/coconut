#ifndef _COCONUT_PULP_RENDERER_MATERIAL_MATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MATERIAL_MATERIAL_HPP_

#include <string>

#include <coconut-tools/exceptions/RuntimeError.hpp>
#include <coconut-tools/system/function-name.hpp>

#include "coconut/milk/math/Vector.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

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

	virtual void setDiffuseColour(const milk::math::Vector4d& rgbaColour) {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

	virtual const milk::math::Vector4d& diffuseColour() const {
		throw NotImplemented(CT_FUNCTION_NAME);
	}

};

MAKE_POINTER_DEFINITIONS(Material);

} // namespace material
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_MATERIAL_HPP_ */
