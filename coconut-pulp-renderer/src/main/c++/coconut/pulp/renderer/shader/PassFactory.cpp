#include "PassFactory.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

std::unique_ptr<Pass> detail::PassCreator::doCreate(
	const std::string& passId, milk::graphics::Renderer& graphicsRenderer)
{

}
