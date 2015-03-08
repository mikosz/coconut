#include "Material.hpp"

#include "shader/ShaderFactory.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Material::Material(milk::graphics::Device& graphicsDevice) {
	shader::ShaderFactory factory;
	vertexShader_ = factory.createShader(graphicsDevice, "VS");
	pixelShader_ = factory.createShader(graphicsDevice, "PS");
}

void Material::bind(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext) {
	renderingContext.material = this;

	vertexShader_->bind(graphicsDevice, renderingContext);
	pixelShader_->bind(graphicsDevice, renderingContext);
}
