#ifndef _COCONUT_PULP_RENDERER_MESH_IMPORTER_HPP_
#define _COCONUT_PULP_RENDERER_MESH_IMPORTER_HPP_

#include <string>
#include <tuple>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/milk/fs.hpp"

#include "Mesh.hpp"

namespace coconut {
namespace pulp {
namespace mesh {

class Importer {
public:

	virtual ~Importer() = default;

	Importer(const Importer&) = delete;

	void operator=(const Importer&) = delete;

	virtual Mesh import(
		const milk::fs::RawData& data,
		const milk::FilesystemContext& filesystemContext
		) = 0;

protected:

	Importer() = default;

};

CCN_MAKE_POINTER_DEFINITIONS(Importer);

} // namespace mesh
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MESH_IMPORTER_HPP_ */
