#ifndef _COCONUT_PULP_RENDERER_MODEL_MESH_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_MESH_HPP_

#include <vector>
#include <string>

#include "coconut/milk/math/Vector.hpp"

#include "coconut/milk/fs.hpp"

// TODO: make one include file
#include <coconut-tools/serialisation/Serialiser.hpp>
#include <coconut-tools/serialisation/Deserialiser.hpp>
#include <coconut-tools/serialisation/make-serialisable-macro.hpp>

#include "coconut/milk/graphics/PrimitiveTopology.hpp"
#include "coconut/milk/graphics/Sampler.hpp"

namespace coconut {
namespace pulp {
namespace model {

struct Mesh {
public:

	struct VertexDescriptor {

		size_t positionIndex;

		size_t normalIndex;

		size_t textureCoordinateIndex;

	};

	struct Instance { // TODO: temp, refactor along with material handling - just for the grass demo p.o.c.

		milk::math::Vector4d patchPosition;

	};

	struct DrawGroup {

		milk::graphics::PrimitiveTopology primitiveTopology;

		std::vector<VertexDescriptor> vertices;

		std::vector<size_t> indices;

		std::string materialId;

		std::vector<Instance> instances;

	};

	class VertexIterator {
	public:

		VertexIterator(const Mesh& data, const DrawGroup& drawGroup);

		void next();

		bool atEnd();

		const Mesh& data() const {
			return data_;
		}

		const DrawGroup& drawGroup() const {
			return drawGroup_;
		}

		size_t index() const {
			return index_;
		}

		const VertexDescriptor& vertexDescriptor() const {
			return drawGroup_.vertices[index_];
		}

	private:

		const Mesh& data_;

		const DrawGroup& drawGroup_;

		size_t index_ = 0u;

	};

	class InstanceIterator { // TODO: temp, merge with VertexIterator
	public:

		InstanceIterator(const Mesh& data, const DrawGroup& drawGroup);

		void next();

		bool atEnd();

		const Mesh& data() const {
			return data_;
		}

		const DrawGroup& drawGroup() const {
			return drawGroup_;
		}

		size_t index() const {
			return index_;
		}

		const Instance& instance() const {
			return drawGroup_.instances[index_];
		}

	private:

		const Mesh& data_;

		const DrawGroup& drawGroup_;

		size_t index_ = 0u;

	};

	std::vector<milk::math::Vector3d> positions;

	std::vector<milk::math::Vector3d> normals;

	std::vector<milk::math::Vector2d> textureCoordinates;

	std::vector<DrawGroup> drawGroups;

	void generateNormals(); // TODO: if Data has functions, it should be a class

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Mesh::DrawGroup, drawGroup) {
	serialiser(SerialiserType::Label("primitiveTopology"), drawGroup.primitiveTopology);
	serialiser(SerialiserType::Label("vertices"), drawGroup.vertices);
	serialiser(SerialiserType::Label("indices"), drawGroup.indices);
	serialiser(SerialiserType::Label("materialId"), drawGroup.materialId);
}

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Mesh::VertexDescriptor, vertexDescriptor) {
	serialiser(SerialiserType::Label("positionIndex"), vertexDescriptor.positionIndex);
	serialiser(SerialiserType::Label("normalIndex"), vertexDescriptor.normalIndex);
	serialiser(SerialiserType::Label("textureCoordinateIndex"), vertexDescriptor.textureCoordinateIndex);
}

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Mesh, data) {
	serialiser(SerialiserType::Label("positions"), data.positions);
	serialiser(SerialiserType::Label("normals"), data.normals);
	serialiser(SerialiserType::Label("textureCoordinates"), data.textureCoordinates);
	serialiser(SerialiserType::Label("drawGroups"), data.drawGroups);
	serialiser(SerialiserType::Label("rasteriserConfiguration"), data.rasteriserConfiguration);
}

} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_MESH_HPP_ */
