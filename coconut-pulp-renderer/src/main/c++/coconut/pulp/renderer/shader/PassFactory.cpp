#include "PassFactory.hpp"

#include <coconut-tools/logger.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.PASS_FACTORY");

} // anonymous namespace

std::unique_ptr<Pass> detail::PassCreator::doCreate(
	const std::string& id, milk::graphics::Renderer& graphicsRenderer)
{
	CT_LOG_INFO << "Creating shader pass: \"" << id << "\"";

	inputLayoutFactory_.registerCompiledShader("sprite.v", "Debug/sprite.v.cso");

	detail::ShaderCreator::CompiledShaderInfo info; // TODO: must expose this type or use different param to constructor (d'uh)

	info.compiledShaderPath = "Debug/sprite.v.cso";
	info.shaderType = milk::graphics::ShaderType::VERTEX;
	shaderFactory_.registerCompiledShader("sprite.v", info);

	info.compiledShaderPath = "Debug/sprite.p.cso";
	info.shaderType = milk::graphics::ShaderType::PIXEL;
	shaderFactory_.registerCompiledShader("sprite.p", info);

	return std::make_unique<Pass>(
		inputLayoutFactory_.create(id + ".v", graphicsRenderer),
		std::dynamic_pointer_cast<VertexShader>(shaderFactory_.create(id + ".v", graphicsRenderer)),
		std::dynamic_pointer_cast<PixelShader>(shaderFactory_.create(id + ".p", graphicsRenderer))
		);
}
