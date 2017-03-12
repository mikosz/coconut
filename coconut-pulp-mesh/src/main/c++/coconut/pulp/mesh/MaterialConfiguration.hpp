#ifndef _COCONUT_PULP_RENDERER_MESH_MATERIALCONFIGURATION_HPP_
#define _COCONUT_PULP_RENDERER_MESH_MATERIALCONFIGURATION_HPP_

#include <string>
#include <tuple>
#include <unordered_map>

#include <boost/any.hpp>

#include <coconut-tools/serialisation.hpp>
#include <coconut-tools/enum.hpp>

#include <coconut-tools/utils/Range.hpp>

#include "coconut/milk/fs/Path.hpp"

#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/milk/graphics/RenderState.hpp"

#include "coconut/pulp/primitive.hpp"

namespace coconut {
namespace pulp {
namespace mesh {

class MaterialConfiguration {
public:

	CCN_MEMBER_ENUM(PassType,
		(OPAQUE)
		(TRANSPARENT)
	);

	static const std::string AMBIENT_COLOUR_PROPERTY;
	static const std::string DIFFUSE_COLOUR_PROPERTY;
	static const std::string SPECULAR_COLOUR_PROPERTY;
	static const std::string SPECULAR_EXPONENT_PROPERTY;

	static const std::string DIFFUSE_MAP_TEXTURE;

	using Properties = std::unordered_map<std::string, Primitive>;

	using Texture = std::tuple<milk::fs::AbsolutePath, milk::graphics::Sampler::Configuration>;

	using Textures = std::unordered_map<std::string, Texture>;

	MaterialConfiguration& addTexture(
		std::string textureName,
		milk::fs::AbsolutePath path,
		milk::graphics::Sampler::Configuration samplerConfiguration
		)
	{
		textures_.emplace(textureName, std::make_tuple(std::move(path), std::move(samplerConfiguration)));
		return *this;
	}

	PassType& passType() {
		return passType_;
	}

	const PassType passType() const {
		return passType_;
	}

	std::string& shaderName() {
		return shaderName_;
	}

	const std::string& shaderName() const {
		return shaderName_;
	}

	milk::graphics::RenderState::Configuration& renderStateConfiguration() {
		return renderStateConfiguration_;
	}

	const milk::graphics::RenderState::Configuration& renderStateConfiguration() const {
		return renderStateConfiguration_;
	}

	Properties& properties() {
		return properties_;
	}

	const Properties& properties() const {
		return properties_;
	}

	const Textures& textures() const {
		return textures_;
	}

private:

	PassType passType_;

	std::string shaderName_;

	milk::graphics::RenderState::Configuration renderStateConfiguration_;

	Properties properties_;

	Textures textures_;

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, MaterialConfiguration, materialConfiguration) {
	serialiser(SerialiserType::Label("passType"), materialConfiguration.passType());
	serialiser(SerialiserType::Label("shaderName"), materialConfiguration.shaderName());
	serialiser(SerialiserType::Label("renderStateConfiguration"), materialConfiguration.renderStateConfiguration());
	serialiser(SerialiserType::Label("properties"), materialConfiguration.properties());
	serialiser(SerialiserType::Label("textures"), materialConfiguration.textures());
}

} // namespace mesh

using mesh::MaterialConfiguration;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MESH_MATERIALCONFIGURATION_HPP_ */
