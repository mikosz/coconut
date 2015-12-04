#ifndef _COCONUT_PULP_RENDERER_MODEL_LOADER_OBJ_MODEL_LOADER_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_LOADER_OBJ_MODEL_LOADER_HPP_

#include <memory>
#include <iosfwd>

#include "ModelLoader.hpp"
#include "ObjModelParser.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace model_loader {

class ObjModelLoader : public ModelLoader {
public:

	typedef std::unique_ptr<std::istream> IStreamPtr;

	typedef ObjModelParser::MaterialFileOpener MaterialFileOpener;

	typedef std::shared_ptr<MaterialFileOpener> MaterialFileOpenerPtr;

	ObjModelLoader(IStreamPtr is, MaterialFileOpenerPtr materialFileOpener) :
		is_(std::move(is)),
		materialFileOpener_(materialFileOpener)
	{
	}

	void load(ModelDataListener& modelDataListener, milk::graphics::Device& graphicsDevice) override;

private:

	IStreamPtr is_;

	MaterialFileOpenerPtr materialFileOpener_;

};

MAKE_POINTER_DEFINITIONS(ModelLoader);

} // namespace model_loader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_LOADER_OBJ_MODEL_LOADER_HPP_ */
