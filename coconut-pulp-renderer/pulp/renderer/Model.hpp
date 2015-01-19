#ifndef _COCONUT_PULP_RENDERER_MODEL_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_HPP_

#include <memory>
#include <unordered_map>

#include "milk/graphics/Buffer.hpp"
#include "milk/graphics/Device.hpp"
#include "milk/graphics/VertexInterface.hpp"

#include "milk/utils/MakePointerDefinitionsMacro.hpp"

#include "model_loader/ModelLoader.hpp"
#include "shader/Shader.hpp"
#include "RenderingContext.hpp"
#include "Material.hpp"
#include "DrawGroup.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Model {
public:

	Model(milk::graphics::Device& graphicsDevice, model_loader::ModelLoader& loader);

	void render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext);

private:

	struct VertexData : public milk::graphics::VertexInterface {

		milk::math::Vector3d position_;

		milk::math::Vector2d textureCoordinate_;

		std::string materialName_;

		milk::math::Vector4d ambientColour_;

		milk::math::Vector4d diffuseColour_;

		milk::math::Vector4d specularColour_;

		float specularExponent_;

		milk::math::Vector3d position() const override {
			return position_;
		}

		milk::math::Vector2d textureCoordinate() const override {
			return textureCoordinate_;
		}

		std::string materialName() const override {
			return materialName_;
		}

		milk::math::Vector4d ambientColour() const override {
			return ambientColour_;
		}

		milk::math::Vector4d diffuseColour() const override {
			return diffuseColour_;
		}

		milk::math::Vector4d specularColour() const override {
			return specularColour_;
		}

		float specularExponent() const override {
			return specularExponent_;
		}

	};

	class DataListener : public model_loader::ModelDataListener {
	public:

		DataListener(Model& model, milk::graphics::Device& graphicsDevice);

		void setVertexPosition(const milk::math::Vector3d& position) override;

		void setVertexTextureCoordinate(const milk::math::Vector2d& textureCoordinate) override;

		void setVertexNormal(const milk::math::Vector3d& normal) override;

		void setVertexNormalNeedsCalculation() override;

		void setMaterialName(const std::string& materialName) override;

		void setAmbientColour(const milk::math::Vector4d& rgbaColour) override;

		void setDiffuseColour(const milk::math::Vector4d& rgbaColour) override;

		void setSpecularColour(const milk::math::Vector4d& rgbaColour) override;

		void setSpecularExponent(float specularExponent) override;

		void endVertex() override;

		void endFace() override;

		void endSmoothingGroup(milk::graphics::PrimitiveTopology primitiveTopology) override;

		void endObject() override;

		void endModel() override;

	private:

		milk::graphics::Device& graphicsDevice_;
			
		Model& model_;

		DrawGroup::Data currentGroupData_;

		VertexData currentVertexData_;

	};

	typedef std::unordered_multimap<MaterialSharedPtr, DrawGroupSharedPtr> DrawGroupsByMaterial;

	DrawGroupsByMaterial drawGroupsByMaterial_;

	friend class DataListener;

};

MAKE_POINTER_DEFINITIONS(Model);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_HPP_ */
