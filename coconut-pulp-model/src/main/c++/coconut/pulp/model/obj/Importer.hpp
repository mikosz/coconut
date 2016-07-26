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

	using MaterialFileOpener = Parser::MaterialFileOpener;

	using MaterialFileOpenerPtr = std::unique_ptr<MaterialFileOpener>;

	Importer(MaterialFileOpenerPtr materialFileOpener) :
		materialFileOpener_(std::move(materialFileOpener))
	{
	}

	Data import(std::istream& is, std::string name) override;

protected:

	Importer() = default;

private:

	MaterialFileOpenerPtr materialFileOpener_;

};

CCN_MAKE_POINTER_DEFINITIONS(Importer);

} // namespace obj
} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_OBJ_IMPORTER_HPP_ */
