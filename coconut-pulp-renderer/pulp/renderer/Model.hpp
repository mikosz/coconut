#ifndef _COCONUT_PULP_RENDERER_MODEL_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_HPP_

#include <memory>
#include <unordered_map>

#include "milk/graphics/Buffer.hpp"
#include "milk/graphics/Device.hpp"
#include "milk/graphics/PrimitiveTopology.hpp"

#include "milk/utils/MakePointerDefinitionsMacro.hpp"

#include "model_loader/ModelLoader.hpp"
#include "shader/Shader.hpp"
#include "RenderingContext.hpp"
#include "Material.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Model {
public:

	struct SmoothingGroup {

		milk::graphics::PrimitiveTopology primitiveTopology;

		size_t firstIndex;

		size_t indexCount;

	};

	Model(milk::graphics::Device& device, model_loader::ModelLoader& loader);

	void render(milk::graphics::Device& devices, RenderingContext renderingContext);

private:

	class DataListener : public model_loader::ModelDataListener {
	public:

		DataListener(Model& model);

		void newObject() override;

		void newSmoothingGroup(milk::graphics::PrimitiveTopology primitiveTopology) override;

		void newFace() override;

		void newVertex() override;

		void setVertexPosition(const milk::math::Vector3d& position) override;

		void setVertexTextureCoordinate(const milk::math::Vector2d& textureCoordinate) override;

		void setVertexNormal(const milk::math::Vector3d& normal) override;

		void setVertexNormalNeedsCalculation() override;

		void endModel() override;

	private:

		Model& model_;

	};

	typedef std::unordered_multimap<MaterialSharedPtr, SmoothingGroup> SmoothingGroupsByMaterial;

	milk::graphics::Buffer vertexBuffer_;

	milk::graphics::Buffer indexBuffer_;

	SmoothingGroupsByMaterial smoothingGroupsByMaterial_;

	friend class DataListener;

};

MAKE_POINTER_DEFINITIONS(Model);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_HPP_ */
