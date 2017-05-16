#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_

#include <vector>
#include <unordered_map>

#include "coconut/milk/graphics/Shader.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/pulp/mesh/MaterialConfiguration.hpp"

#include "Resource.hpp"
#include "ConstantBuffer.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class PassContext;
class Actor;
class Scene;
class DrawCommand;

namespace shader {

class UnknownShader {
public:

	using ConstantBuffers = std::vector<ConstantBuffer>;

	using Resources = std::vector<Resource>;

	virtual ~UnknownShader() = default;

};

namespace detail {

template <class GraphicsShaderType>
class Shader : public UnknownShader {
public:

	Shader(
		GraphicsShaderType shaderData,
		ConstantBuffers constantBuffers,
		Resources resources
		) :
		shaderData_(std::move(shaderData)),
		constantBuffers_(std::move(constantBuffers)),
		resources_(std::move(resources))
	{
	}

	void bind(DrawCommand& drawCommand, const Properties& properties) {
		for (auto& buffer : constantBuffers_) {
			buffer.bind(drawCommand, properties);
		}

		for (auto resource : resources_) {
			resource.bind(drawCommand, properties);
		}
	}

	GraphicsShaderType& shaderData() {
		return shaderData_;
	}

private:

	GraphicsShaderType shaderData_;

	ConstantBuffers constantBuffers_;

	Resources resources_;

};

} // namespace detail

using VertexShader = detail::Shader<milk::graphics::VertexShader>;
using GeometryShader = detail::Shader<milk::graphics::GeometryShader>;
using HullShader = detail::Shader<milk::graphics::HullShader>;
using DomainShader = detail::Shader<milk::graphics::DomainShader>;
using PixelShader = detail::Shader<milk::graphics::PixelShader>;

CT_MAKE_POINTER_DEFINITIONS(VertexShader);
CT_MAKE_POINTER_DEFINITIONS(GeometryShader);
CT_MAKE_POINTER_DEFINITIONS(HullShader);
CT_MAKE_POINTER_DEFINITIONS(DomainShader);
CT_MAKE_POINTER_DEFINITIONS(PixelShader);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_ */
