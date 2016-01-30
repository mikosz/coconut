#ifndef _COCONUT_PULP_RENDERER_MODEL_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_HPP_

#include <memory>
#include <unordered_map>

#include "coconut/milk/graphics/Buffer.hpp"
#include "coconut/milk/graphics/Device.hpp"
#include "coconut/milk/graphics/PrimitiveTopology.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "material/MaterialLibrary.hpp"
#include "model_loader/ModelLoader.hpp"
#include "shader/Shader.hpp"
#include "RenderingContext.hpp"
#include "DrawGroup.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Model {
public:

	Model(
		milk::graphics::Device& graphicsDevice,
		model_loader::ModelLoader& loader,
		const milk::graphics::InputLayoutDescription& inputLayoutDescription
		);

	void render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext);

private:

	struct VertexData : public milk::graphics::VertexInterface {

		milk::math::Vector3d position_;

		milk::math::Vector2d textureCoordinate_;

		milk::math::Vector3d normal_;

		std::string materialName_;

		milk::math::Vector3d position() const override {
			return position_;
		}

		milk::math::Vector2d textureCoordinate() const override {
			return textureCoordinate_;
		}

		milk::math::Vector3d normal() const override {
			return normal_;
		}

		std::string materialName() const override {
			return materialName_;
		}

	};

	typedef std::unordered_multimap<material::ConstMaterialSharedPtr, DrawGroupSharedPtr> DrawGroupsByMaterial;

	material::MaterialLibrary materialLibrary_;

	DrawGroupsByMaterial drawGroupsByMaterial_;

	friend class ModelDataListener;

};

MAKE_POINTER_DEFINITIONS(Model);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_HPP_ */
