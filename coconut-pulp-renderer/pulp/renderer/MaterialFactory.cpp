#include "MaterialFactory.hpp"

#include "milk/graphics/FlexibleInputLayoutDescription.hpp"

#include "shader/ShaderFactory.hpp"
#include "shader/ShaderSet.hpp"
#include "Material.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

MaterialSharedPtr MaterialFactory::createMaterial(
	milk::graphics::Device& graphicsDevice,
	const MaterialId& materialId
	) {
	milk::graphics::FlexibleInputLayoutDescriptionSharedPtr inputLayout(new milk::graphics::FlexibleInputLayoutDescription());
	inputLayout->push(
		milk::graphics::FlexibleInputLayoutDescription::ElementSharedPtr(
			new milk::graphics::FlexibleInputLayoutDescription::PositionElement(0, milk::graphics::FlexibleInputLayoutDescription::Format::R32G32B32A32_FLOAT)
			)
		);

	shader::ShaderFactory shaderFactory;
	shader::ShaderSetSharedPtr shaderSet(new shader::ShaderSet(
		shaderFactory.createShader(graphicsDevice, "VS"),
		shaderFactory.createShader(graphicsDevice, "PS"),
		inputLayout		
		));

	return MaterialSharedPtr(new Material(graphicsDevice, shaderSet));
}
