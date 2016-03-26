#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_

#include <string>
#include <unordered_map>

#include "coconut/milk/graphics/Device.hpp"

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

	PassSharedPtr createShaderPass(milk::graphics::Device& graphicsDevice, PassId passId);

private:

	using PassCache = std::unordered_map<PassId, PassSharedPtr>;

	milk::graphics::InputLayoutUniquePtr createInputLayoutDescription(
		milk::graphics::Device& graphicsDevice,
		InputLayoutDescriptionId inputlayoutDescriptionId,
		const std::vector<char>& binaryShaderData
		);

	ShaderSharedPtr createShader(milk::graphics::Device& graphicsDevice, ShaderId shaderId);

	ShaderSharedPtr createVertexShader(milk::graphics::Device& graphicsDevice, ShaderId shaderId);

	PassCache passCache_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_ */
