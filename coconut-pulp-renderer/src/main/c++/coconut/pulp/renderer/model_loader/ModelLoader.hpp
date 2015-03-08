#ifndef _COCONUT_PULP_RENDERER_MODEL_LOADER_MODEL_LOADER_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_LOADER_MODEL_LOADER_HPP_

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "ModelDataListener.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace model_loader {

class ModelLoader {
public:

	virtual void load(ModelDataListener& modelDataListener) = 0;

protected:

	virtual ~ModelLoader() {
	}

};

MAKE_POINTER_DEFINITIONS(ModelLoader);

} // namespace model_loader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_LOADER_MODEL_LOADER_HPP_ */
