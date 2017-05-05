#ifndef _COCONUT_PULP_RENDERER_MATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MATERIAL_HPP_

#include <string>
#include <memory>
#include <unordered_map>

#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/fs/FilesystemContext.hpp"
#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/RenderState.hpp"
#include "coconut/milk/graphics/Texture.hpp"
#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/pulp/mesh/MaterialConfiguration.hpp"
#include "shader/Property.hpp"
#include "shader/PassFactory.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Material {
public:

	Material(
		milk::graphics::Renderer graphicsRenderer,
		shader::PassFactory& passFactory,
		const milk::fs::FilesystemContext& filesystemContext,
		mesh::MaterialConfiguration materialConfiguration
		);

	shader::Pass& shaderPass() {
		return *pass_;
	}

	const milk::graphics::RenderState& renderState() const {
		return renderState_;
	}

	const shader::Properties& shaderProperties() const {
		return shaderProperties_;
	}

private:

	mesh::MaterialConfiguration::PassType passType_;

	shader::PassSharedPtr pass_;

	milk::graphics::RenderState renderState_;

	shader::Properties shaderProperties_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_HPP_ */
