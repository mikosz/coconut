#ifndef _COCONUT_PULP_RENDERER_MODEL_LOADER_MODEL_LOADER_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_LOADER_MODEL_LOADER_HPP_

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"
#include "coconut/milk/graphics/Device.hpp"

#include "ModelDataListener.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace model_loader {

class ModelLoader {
public:

	virtual ~ModelLoader() = default;

	ModelLoader(const ModelLoader&) = delete;

	void operator=(const ModelLoader&) = delete;

	virtual void load(ModelDataListener& modelDataListener, milk::graphics::Device& graphicsDevice) = 0;

protected:

	ModelLoader() = default;

};

MAKE_POINTER_DEFINITIONS(ModelLoader);

} // namespace model_loader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_LOADER_MODEL_LOADER_HPP_ */
