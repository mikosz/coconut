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

#include "shader/PassFactory.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Material {
public:

	using Texture = std::tuple<milk::graphics::Texture2d, milk::graphics::Sampler>;

	class NoSuchProperty : public coconut_tools::exceptions::RuntimeError {
	public:
	
		NoSuchProperty(const std::string& key) :
			coconut_tools::exceptions::RuntimeError("Material doesn't have property " + key)
		{
		}
	
	};

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

	const bool hasProperty(const std::string& key) const {
		return properties_.count(key) != 0;
	}

	const Primitive& property(const std::string& key) const;

	const bool hasTexture(const std::string& key) const {
		return textures_.count(key) != 0;
	}

	const Texture& texture(const std::string& key) const;

private:

	using Textures = std::unordered_map<std::string, Texture>;

	mesh::MaterialConfiguration::PassType passType_;

	shader::PassSharedPtr pass_;

	milk::graphics::RenderState renderState_;

	mesh::MaterialConfiguration::Properties properties_;

	Textures textures_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_HPP_ */
