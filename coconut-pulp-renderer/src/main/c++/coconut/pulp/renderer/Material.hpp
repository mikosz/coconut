#ifndef _COCONUT_PULP_RENDERER_MATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MATERIAL_HPP_

#include <string>

#include <coconut-tools/enum.hpp>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/milk/system/cleanup-windows-macros.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

CCN_ENUM(ShaderPassType,
	(OPAQUE)
	(TRANSPARENT)
	(GRASS)
	);

class Material {
public:

	Material(ShaderPassType shaderPassType) :
		shaderPassType_(shaderPassType)
	{
	}

	Material(const Material&) = delete;

	void operator=(const Material&) = delete;

	virtual ~Material() = default;

	virtual const std::string& type() const = 0;

	ShaderPassType shaderPassType() const {
		return shaderPassType_;
	}

private:

	ShaderPassType shaderPassType_;

};

CCN_MAKE_POINTER_DEFINITIONS(Material);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_HPP_ */
