#include "DirectionalLight.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::lighting;

shader::ReflectiveInterface<DirectionalLight>::ReflectiveInterface() {
	// TODO: address of return value (would rather pass value)...
	// TODO: also, would rather return a pointer to member field
	emplaceMethod("direction", [](const DirectionalLight& light) { return &light.direction(); });
	emplaceMethod("ambientColour", [](const DirectionalLight& light) { return &light.ambientColour(); });
	emplaceMethod("diffuseColour", [](const DirectionalLight& light) { return &light.diffuseColour(); });
	emplaceMethod("specularColour", [](const DirectionalLight& light) { return &light.specularColour(); });
}

math::Transform DirectionalLight::viewProjection() const {
	const auto view = math::Transform();

	const auto projection = math::Transform::OrthographicProjection();

	return view << projection;
}
