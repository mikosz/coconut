#ifndef _COCONUT_PULP_RENDERER_MODEL_OBJ_IMPORTER_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_OBJ_IMPORTER_HPP_

#include "coconut/pulp/model/Importer.hpp"

#include "Parser.hpp"

namespace coconut {
namespace pulp {
namespace model {
namespace obj {

class Importer : public model::Importer {
public:

	Importer() = default;

	Data import(
		const std::string& name,
		std::istream& is,
		const milk::FilesystemContext& filesystemContext
		) override;

};

CCN_MAKE_POINTER_DEFINITIONS(Importer);

} // namespace obj
} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_OBJ_IMPORTER_HPP_ */
