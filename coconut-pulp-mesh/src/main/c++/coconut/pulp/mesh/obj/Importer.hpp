#ifndef _COCONUT_PULP_RENDERER_MESH_OBJ_IMPORTER_HPP_
#define _COCONUT_PULP_RENDERER_MESH_OBJ_IMPORTER_HPP_

#include "coconut/pulp/mesh/Importer.hpp"

#include "Parser.hpp"

namespace coconut {
namespace pulp {
namespace mesh {
namespace obj {

class Importer : public mesh::Importer {
public:

	Importer() = default;

	Mesh import(
		const milk::fs::RawData& data,
		const milk::FilesystemContext& filesystemContext
		) override;

};

CT_MAKE_POINTER_DEFINITIONS(Importer);

} // namespace obj
} // namespace mesh
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MESH_OBJ_IMPORTER_HPP_ */
