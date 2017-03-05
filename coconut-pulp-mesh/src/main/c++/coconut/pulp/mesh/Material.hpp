#ifndef _COCONUT_PULP_RENDERER_MESH_MATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MESH_MATERIAL_HPP_

#include <string>
#include <tuple>
#include <unordered_map>

#include <boost/any.hpp>

// TODO: make one include file
#include <coconut-tools/serialisation/Serialiser.hpp>
#include <coconut-tools/serialisation/Deserialiser.hpp>
#include <coconut-tools/serialisation/make-serialisable-macro.hpp>

#include <coconut-tools/enum.hpp>

#include <coconut-tools/utils/Range.hpp>

#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/fs/Path.hpp"

#include "coconut/milk/graphics/Sampler.hpp"

#include "coconut/pulp/primitive.hpp"

#include "RenderStateConfiguration.hpp"

namespace coconut {
namespace pulp {
namespace mesh {

class Material {
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

	class NoSuchProperty : public coconut_tools::exceptions::RuntimeError {
	public:

		NoSuchProperty(const std::string& name) :
			coconut_tools::exceptions::RuntimeError("No such property: \"" + name + '"')
		{
		}

	};

	using Properties = std::unordered_map<std::string, Primitive>;

	using Texture = std::tuple<milk::fs::AbsolutePath, milk::graphics::Sampler::Configuration>;

	using Textures = std::unordered_map<std::string, Texture>;

	Material& set(const std::string& property, Primitive value);

	const Primitive& get(const std::string& property) const;

	Material& addTexture(
		std::string textureName,
		milk::fs::AbsolutePath path,
		milk::graphics::Sampler::Configuration samplerConfiguration
		);

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

	RenderStateConfiguration& renderStateConfiguration() {
		return renderStateConfiguration_;
	}

	const RenderStateConfiguration& renderStateConfiguration() const {
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

	RenderStateConfiguration renderStateConfiguration_;

	Properties properties_;

	Textures textures_;

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Material, material) {
	serialiser(SerialiserType::Label("passType"), material.passType());
	serialiser(SerialiserType::Label("shaderName"), material.shaderName());
	serialiser(SerialiserType::Label("renderStateConfiguration"), material.renderStateConfiguration());
	serialiser(SerialiserType::Label("properties"), material.properties());
	serialiser(SerialiserType::Label("textures"), material.textures());
}

} // namespace mesh

using mesh::Material;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MESH_MATERIAL_HPP_ */
