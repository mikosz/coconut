#include "Material.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Material::Material(
	milk::graphics::Renderer graphicsRenderer,
	shader::PassFactory& passFactory,
	mesh::MaterialConfiguration materialConfiguration
	) :
	passType_(materialConfiguration.passType()),
	pass_(passFactory.create(materialConfiguration.shaderName())),
	renderState_(graphicsRenderer, materialConfiguration.renderStateConfiguration()),
	properties_(std::move(materialConfiguration.properties()))
{
}
