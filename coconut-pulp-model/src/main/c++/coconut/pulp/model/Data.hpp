#ifndef _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_

#include <vector>
#include <string>

#include "coconut/milk/math/Vector.hpp"

#include "coconut/milk/fs.hpp"

// TODO: make one include file
#include <coconut-tools/serialisation/Serialiser.hpp>
#include <coconut-tools/serialisation/Deserialiser.hpp>
#include <coconut-tools/serialisation/make-serialisable-macro.hpp>

#include "coconut/milk/graphics/PrimitiveTopology.hpp"
#include "coconut/milk/graphics/Rasteriser.hpp"
#include "coconut/milk/graphics/Sampler.hpp"

namespace coconut {
namespace pulp {
namespace model {

struct Data {
public:

	struct PhongMaterial {

		std::string name;

		milk::math::Vector4d ambientColour;

		milk::math::Vector4d diffuseColour;

		milk::math::Vector4d specularColour;

		float specularExponent;

		milk::AbsolutePath diffuseMap;

		milk::graphics::Sampler::Configuration diffuseMapSamplerConfiguration;

	};

	struct VertexDescriptor {

		size_t positionIndex;

		size_t normalIndex;

		size_t textureCoordinateIndex;

	};

	struct DrawGroup {

		milk::graphics::PrimitiveTopology primitiveTopology;

		std::vector<VertexDescriptor> vertices;

		std::vector<size_t> indices;

		std::string materialId;

	};

	std::vector<PhongMaterial> phongMaterials;

	std::vector<milk::math::Vector3d> positions;

	std::vector<milk::math::Vector3d> normals;

	std::vector<milk::math::Vector2d> textureCoordinates;

	std::vector<DrawGroup> drawGroups;

	milk::graphics::Rasteriser::Configuration rasteriserConfiguration;

	void generateNormals(); // TODO: if Data has functions, it should be a class

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Data::PhongMaterial, material) {
	serialiser(SerialiserType::Label("name"), material.name);
	serialiser(SerialiserType::Label("ambientColour"), material.ambientColour);
	serialiser(SerialiserType::Label("diffuseColour"), material.diffuseColour);
	serialiser(SerialiserType::Label("specularColour"), material.specularColour);
	serialiser(SerialiserType::Label("specularExponent"), material.specularExponent);
	serialiser(SerialiserType::Label("diffuseMap"), material.diffuseMap);
	serialiser(SerialiserType::Label("diffuseMapSamplerConfiguration"), material.diffuseMapSamplerConfiguration);
}

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Data::DrawGroup, drawGroup) {
	serialiser(SerialiserType::Label("primitiveTopology"), drawGroup.primitiveTopology);
	serialiser(SerialiserType::Label("vertices"), drawGroup.vertices);
	serialiser(SerialiserType::Label("indices"), drawGroup.indices);
	serialiser(SerialiserType::Label("materialId"), drawGroup.materialId);
}

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Data::VertexDescriptor, vertexDescriptor) {
	serialiser(SerialiserType::Label("positionIndex"), vertexDescriptor.positionIndex);
	serialiser(SerialiserType::Label("normalIndex"), vertexDescriptor.normalIndex);
	serialiser(SerialiserType::Label("textureCoordinateIndex"), vertexDescriptor.textureCoordinateIndex);
}

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Data, data) {
	serialiser(SerialiserType::Label("phongMaterials"), data.phongMaterials);
	serialiser(SerialiserType::Label("positions"), data.positions);
	serialiser(SerialiserType::Label("normals"), data.normals);
	serialiser(SerialiserType::Label("textureCoordinates"), data.textureCoordinates);
	serialiser(SerialiserType::Label("drawGroups"), data.drawGroups);
	serialiser(SerialiserType::Label("rasteriserConfiguration"), data.rasteriserConfiguration);
}

} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_ */
