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
	
	detail::ShaderCreator::CompiledShaderInfo info; // TODO: must expose this type or use different param to constructor (d'uh)

	info.compiledShaderPath = "Debug/sprite.v.cso";
	info.shaderType = milk::graphics::ShaderType::VERTEX;
	shaderFactory_.registerCompiledShader("sprite.v", info);

	info.compiledShaderPath = "Debug/sprite.p.cso";
	info.shaderType = milk::graphics::ShaderType::VERTEX;
	shaderFactory_.registerCompiledShader("sprite.p", info);

	shaderFactory_.create(id + ".v", graphicsRenderer);
	// shaderFactory_.create(id + ".p", graphicsRenderer);

	throw "demo endddd";

	return std::unique_ptr<Pass>();
}
