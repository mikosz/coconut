#ifndef _COCONUT_PULP_RENDERER_MESH_MESH_HPP_
#define _COCONUT_PULP_RENDERER_MESH_MESH_HPP_

#include <vector>
#include <unordered_map>

#include "coconut/pulp/math/Vector.hpp"

#include <coconut-tools/serialisation.hpp>

#include "MaterialConfiguration.hpp"
#include "Submesh.hpp"

namespace coconut {
namespace pulp {
namespace mesh {

class Mesh {
public:

	using Submeshes = std::vector<Submesh>;

	using MaterialConfigurations = std::unordered_map<std::string, MaterialConfiguration>;

	Mesh() = default;

	Mesh(Submeshes s, MaterialConfigurations m) :
		submeshes_(std::move(s)),
		materialsConfigurations_(std::move(m))
	{
	}

	Submeshes& submeshes() {
		return submeshes_;
	}

	const Submeshes& submeshes() const {
		return submeshes_;
	}

	MaterialConfigurations& materials() {
		return materialsConfigurations_;
	}

	const MaterialConfigurations& materials() const {
		return materialsConfigurations_;
	}

private:

	Submeshes submeshes_;

	MaterialConfigurations materialsConfigurations_;

};

CT_MAKE_SERIALISABLE(SerialiserType, serialiser, Mesh, mesh) {
	serialiser(SerialiserType::Label("submeshes"), mesh.submeshes());
	serialiser(SerialiserType::Label("materials"), mesh.materials());
}

} // namespace mesh

using mesh::Mesh;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MESH_MESH_HPP_ */
