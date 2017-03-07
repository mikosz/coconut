#ifndef _COCONUT_PULP_RENDERER_MATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MATERIAL_HPP_

#include <string>
#include <memory>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/RenderState.hpp"

#include "coconut/pulp/mesh/MaterialConfiguration.hpp"

#include "shader/PassFactory.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Material {
public:

	Material(
		milk::graphics::Renderer graphicsRenderer,
		shader::PassFactory& passFactory,
		mesh::MaterialConfiguration materialConfiguration
		);

private:

	mesh::MaterialConfiguration::PassType passType_;

	shader::PassSharedPtr pass_;

	milk::graphics::RenderState renderState_;

	mesh::MaterialConfiguration::Properties properties_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_HPP_ */
