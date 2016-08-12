#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_

#include <string>
#include <unordered_map>

#include "coconut/milk/graphics/Renderer.hpp"

#include "Pass.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ShaderFactory {
public:

	using ShaderId = std::string;

	using PassId = std::string;

	using InputLayoutDescriptionId = std::string;

	ShaderFactory();

	PassSharedPtr createShaderPass(milk::graphics::Renderer& graphicsRenderer, const PassId& passId);

private:

	using PassCache = std::unordered_map<PassId, PassSharedPtr>;

	using VertexShaderCache = std::unordered_map<ShaderId, milk::graphics::VertexShader>;

	using PixelShaderCache = std::unordered_map<ShaderId, milk::graphics::PixelShader>;

	PassCache passCache_;

	milk::graphics::InputLayoutUniquePtr createInputLayoutDescription(
		milk::graphics::Renderer& renderer,
		InputLayoutDescriptionId inputLayoutDescriptionId,
		const std::vector<std::uint8_t>& binaryShaderData
		);

	PassSharedPtr makeShaderPass(milk::graphics::Renderer& renderer, const ShaderFactory::PassId& passId);

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_ */
