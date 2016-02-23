#ifndef _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_

#include <vector>
#include <string>

#include "coconut/milk/math/Vector.hpp"

#include "coconut/milk/graphics/PrimitiveTopology.hpp"

#include "coconut/pulp/file-io/Serialiser.hpp"
#include "coconut/pulp/file-io/Deserialiser.hpp"
#include "coconut/pulp/file-io/make-serialisable-macro.hpp"

namespace coconut {
namespace pulp {
namespace model {

struct Data {
public:

	/* TODO:
	 * renderer material is an abstract class designed to incorporate multiple material types (currently only implementation
	 * is PhongMaterial). The struct below only allows Phong material, so there's no way to provide data for other material
	 * types. Either rewrite this, or simplify renderer material.
	 */
	struct Material {

		std::string name; // TODO: necessary?

		milk::math::Vector4d ambientColour;

		milk::math::Vector4d diffuseColour;

		milk::math::Vector4d specularColour;

		float specularExponent;

		std::string diffuseMap; // TODO: string?

	};

	struct VertexDescriptor {

		size_t positionIndex;

		size_t normalIndex;

		size_t textureCoordinateIndex;

	};

	struct DrawGroup {

		milk::graphics::PrimitiveTopology primitiveTopology = milk::graphics::PrimitiveTopology::TRIANGLE_LIST; // TODO!

		std::vector<VertexDescriptor> vertices;

		std::vector<size_t> indices;

		Material material;

	};

	std::vector<milk::math::Vector3d> positions;

	std::vector<milk::math::Vector3d> normals;

	std::vector<milk::math::Vector2d> textureCoordinates;

	std::vector<DrawGroup> drawGroups;

	void generateNormals(); // TODO: if Data has functions, it should be a class

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Data::DrawGroup, drawGroup) {
	serialiser(SerialiserType::Label("primitiveTopology"), drawGroup.primitiveTopology);
	serialiser(SerialiserType::Label("vertices"), drawGroup.vertices);
	serialiser(SerialiserType::Label("indices"), drawGroup.indices);
	serialiser(SerialiserType::Label("material"), drawGroup.material);
}

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Data::Material, material) {
	serialiser(SerialiserType::Label("name"), material.name);
	serialiser(SerialiserType::Label("ambientColour"), material.ambientColour);
	serialiser(SerialiserType::Label("diffuseColour"), material.diffuseColour);
	serialiser(SerialiserType::Label("specularColour"), material.specularColour);
	serialiser(SerialiserType::Label("specularExponent"), material.specularExponent);
	serialiser(SerialiserType::Label("diffuseMap"), material.diffuseMap);
}

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Data::VertexDescriptor, vertexDescriptor) {
	serialiser(SerialiserType::Label("positionIndex"), vertexDescriptor.positionIndex);
	serialiser(SerialiserType::Label("normalIndex"), vertexDescriptor.normalIndex);
	serialiser(SerialiserType::Label("textureCoordinateIndex"), vertexDescriptor.textureCoordinateIndex);
}

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Data, data) {
	serialiser(SerialiserType::Label("positions"), data.positions);
	serialiser(SerialiserType::Label("normals"), data.normals);
	serialiser(SerialiserType::Label("textureCoordinates"), data.textureCoordinates);
	serialiser(SerialiserType::Label("drawGroups"), data.drawGroups);
}

} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_ */
