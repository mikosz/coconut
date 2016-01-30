#ifndef _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_

#include <vector>
#include <string>

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace model {

struct Data {
public:

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

		std::vector<VertexDescriptor> vertices;

		std::vector<size_t> indices;

		Material material;

		bool normalsNeedGeneration;

	};

	std::vector<milk::math::Vector3d> positions;

	std::vector<milk::math::Vector3d> normals;

	std::vector<milk::math::Vector2d> textureCoordinates;

	std::vector<DrawGroup> drawGroups;

};

} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_DATA_HPP_ */
