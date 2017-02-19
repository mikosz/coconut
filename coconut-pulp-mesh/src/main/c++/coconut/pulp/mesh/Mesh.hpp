#ifndef _COCONUT_PULP_RENDERER_MESH_MESH_HPP_
#define _COCONUT_PULP_RENDERER_MESH_MESH_HPP_

#include <vector>

#include "coconut/milk/math/Vector.hpp"

// TODO: make one include file
#include <coconut-tools/serialisation/Serialiser.hpp>
#include <coconut-tools/serialisation/Deserialiser.hpp>
#include <coconut-tools/serialisation/make-serialisable-macro.hpp>

#include "Material.hpp"
#include "Submesh.hpp"

namespace coconut {
namespace pulp {
namespace mesh {

class Mesh {
public:

	using Submeshes = std::vector<Submesh>;

	using Materials = std::vector<Material>;

	Mesh(Submeshes s, Materials m) :
		submeshes_(std::move(s)),
		materials_(std::move(m))
	{
	}

	const Submeshes& submeshes() const {
		return submeshes_;
	}

	const Materials& materials() const {
		return materials_;
	}

private:

	Submeshes submeshes_;

	Materials materials_;

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Mesh, mesh) {
	serialiser(SerialiserType::Label("submeshes"), mesh.submeshes());
	serialiser(SerialiserType::Label("materials"), mesh.materials());
}

} // namespace mesh
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MESH_MESH_HPP_ */
