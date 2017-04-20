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

	using SceneData = std::vector<ConstantBufferSharedPtr<Scene>>;

	using ActorData = std::vector<ConstantBufferSharedPtr<Actor>>;

	using MaterialData = std::vector<ConstantBufferSharedPtr<Material>>;

	using Resources = std::vector<ResourceSharedPtr>;

	virtual ~UnknownShader() = default;

};

namespace detail {

template <class GraphicsShaderType>
class Shader : public UnknownShader {
public:

	Shader(
		GraphicsShaderType shaderData,
		SceneData sceneData,
		ActorData actorData,
		MaterialData materialData,
		Resources resources
		);

	void bind(DrawCommand& drawCommand, const PassContext& passContext) const;

	GraphicsShaderType& shaderData() {
		return shaderData_;
	}

private:

	GraphicsShaderType shaderData_;

	SceneData sceneData_;

	ActorData actorData_;

	MaterialData materialData_;

	Resources resources_;

};

} // namespace detail

using VertexShader = detail::Shader<milk::graphics::VertexShader>;
using GeometryShader = detail::Shader<milk::graphics::GeometryShader>;
using HullShader = detail::Shader<milk::graphics::HullShader>;
using DomainShader = detail::Shader<milk::graphics::DomainShader>;
using PixelShader = detail::Shader<milk::graphics::PixelShader>;

CCN_MAKE_POINTER_DEFINITIONS(VertexShader);
CCN_MAKE_POINTER_DEFINITIONS(GeometryShader);
CCN_MAKE_POINTER_DEFINITIONS(HullShader);
CCN_MAKE_POINTER_DEFINITIONS(DomainShader);
CCN_MAKE_POINTER_DEFINITIONS(PixelShader);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_ */
