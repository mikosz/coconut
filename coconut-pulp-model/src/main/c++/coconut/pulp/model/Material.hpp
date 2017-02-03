#ifndef _COCONUT_PULP_RENDERER_MODEL_MATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_MATERIAL_HPP_

#include <string>
#include <unordered_map>

#include <boost/any.hpp>

// TODO: make one include file
#include <coconut-tools/serialisation/Serialiser.hpp>
#include <coconut-tools/serialisation/Deserialiser.hpp>
#include <coconut-tools/serialisation/make-serialisable-macro.hpp>

#include <coconut-tools/utils/range.hpp>

#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/graphics/Rasteriser.hpp"

namespace coconut {
namespace pulp {
namespace model {

class Material {
public:

	class NoSuchProperty : public coconut_tools::exceptions::RuntimeError {
	public:

		NoSuchProperty(const std::string& name) :
			coconut_tools::exceptions::RuntimeError("No such property: \"" + name + '"')
		{
		}

	};

	using Properties = std::unordered_map<std::string, boost::any>;

	void set(const std::string& property, boost::any value);

	const boost::any& get(const std::string& property) const;

	Properties& properties() {
		return properties_;
	}

	const Properties& properties() const {
		return properties_;
	}

	std::string& shader() {
		return shader_;
	}

	const std::string& shader() const {
		return shader_;
	}

	milk::graphics::Rasteriser::Configuration& rasteriserConfiguration() {
		return rasteriserConfiguration_;
	}

	const milk::graphics::Rasteriser::Configuration& rasteriserConfiguration() const {
		return rasteriserConfiguration_;
	}

private:

	std::string shader_;

	Properties properties_;

	milk::graphics::Rasteriser::Configuration rasteriserConfiguration_;

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Material, material) {
	serialiser(SerialiserType::Label("shader"), material.shader());
	serialiser(SerialiserType::Label("properties"), material.properties());
}

} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_MATERIAL_HPP_ */
