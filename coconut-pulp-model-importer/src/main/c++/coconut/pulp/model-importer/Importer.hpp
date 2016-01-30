#ifndef _COCONUT_PULP_RENDERER_MODEL_IMPORTER_IMPORTER_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_IMPORTER_IMPORTER_HPP_

#include <iosfwd>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "Data.hpp"

namespace coconut {
namespace pulp {
namespace model_importer {

class Importer {
public:

	virtual ~Importer() = default;

	Importer(const Importer&) = delete;

	void operator=(const Importer&) = delete;

	virtual Data import(std::istream& is) = 0;

protected:

	Importer() = default;

};

MAKE_POINTER_DEFINITIONS(Importer);

} // namespace model_importer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_IMPORTER_IMPORTER_HPP_ */
