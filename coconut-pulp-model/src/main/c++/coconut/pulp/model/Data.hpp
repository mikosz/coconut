#ifndef _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_

#include <vector>
#include <string>

#include "coconut/milk/math/Vector.hpp"
#include "coconut/milk/graphics/PrimitiveTopology.hpp"

#include "coconut/pulp/file-io/Serialiser.hpp"

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

		milk::graphics::PrimitiveTopology primitiveTopology;

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

void serialise(file_io::Serialiser& serialiser, const Data& data);

} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_ */
