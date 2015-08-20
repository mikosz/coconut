#include "Material.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Material::Material(milk::graphics::Device& graphicsDevice, shader::ShaderSetSharedPtr shaderSet) :
	shaderSet_(shaderSet)
{
}

void Material::bind(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext) {
	renderingContext.material = this;
	shaderSet_->bind(graphicsDevice, renderingContext);
}
