#ifndef _COCONUT_PULP_RENDERER_MESH_SUBMESH_HPP_
#define _COCONUT_PULP_RENDERER_MESH_SUBMESH_HPP_

#include <vector>
#include <string>

// TODO: make one include file
#include <coconut-tools/serialisation/Serialiser.hpp>
#include <coconut-tools/serialisation/Deserialiser.hpp>
#include <coconut-tools/serialisation/make-serialisable-macro.hpp>

#include "coconut/milk/graphics/PrimitiveTopology.hpp"
#include "coconut/milk/graphics/Sampler.hpp"

#include "coconut/pulp/primitive.hpp"

namespace coconut {
namespace pulp {
namespace mesh {

class Submesh {
public:

	struct Vertex {

		Position position;

		Vector normal;

		TextureCoordinate textureCoordinate;

	};

	using Vertices = std::vector<Vertex>;

	using Indices = std::vector<size_t>;

	Submesh(
		Vertices v,
		Indices i,
		std::string materialId,
		milk::graphics::PrimitiveTopology primitiveTopology
		) :
		vertices_(std::move(v)),
		indices_(std::move(i)),
		materialId_(std::move(materialId)),
		primitiveTopology_(primitiveTopology)
	{
	}

	const Vertices& vertices() const {
		return vertices_;
	}

	const Indices& indices() const {
		return indices_;
	}

	const std::string& materialId() const {
		return materialId_;
	}

	milk::graphics::PrimitiveTopology primitiveTopology() const {
		return primitiveTopology_;
	}

private:

	Vertices vertices_;

	Indices indices_;

	std::string materialId_;

	milk::graphics::PrimitiveTopology primitiveTopology_;

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Submesh, submesh) {
	serialiser(SerialiserType::Label("vertices"), submesh.vertices());
	serialiser(SerialiserType::Label("indices"), submesh.indices());
	serialiser(SerialiserType::Label("materialId"), submesh.materialId());
	serialiser(SerialiserType::Label("primitiveTopology"), submesh.primitiveTopology());
}

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Submesh::Vertex, vertex) {
	serialiser(SerialiserType::Label("position"), vertex.position);
	serialiser(SerialiserType::Label("normal"), vertex.normal);
	serialiser(SerialiserType::Label("textureCoordinate"), vertex.textureCoordinate);
}

} // namespace mesh

using mesh::Submesh;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MESH_SUBMESH_HPP_ */
