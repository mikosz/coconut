#ifndef _COCONUT_PULP_MODEL_HPP_
#define _COCONUT_PULP_MODEL_HPP_

#include <memory>

#include "milk/graphics/Buffer.hpp"
#include "milk/graphics/Device.hpp"
#include "milk/graphics/VertexShader.hpp"
#include "milk/graphics/PixelShader.hpp"
#include "milk/graphics/Renderable.hpp"
#include "milk/math/Vector.hpp"
#include "milk/math/Matrix.hpp"
#include "milk/utils/Lazy.hpp"

namespace coconut {
namespace pulp {
namespace model {

class Model : public milk::graphics::Renderable {
public:

	Model(
		milk::graphics::Device& device,
		std::shared_ptr<milk::graphics::VertexShader> vertexShader,
		std::shared_ptr<milk::graphics::PixelShader> pixelShader
		);

	void render(milk::graphics::Device& device) override;

	const milk::math::Matrix& worldTransformation() const override;

	void setTranslation(const milk::math::Vector3d& translation) {
		translation_ = translation;
		worldMatrix_.invalidate();
	}

	const milk::math::Vector3d& getTranslation() const {
		return translation_;
	}

	void setScale(const milk::math::Vector3d& scale) {
		scale_ = scale;
		worldMatrix_.invalidate();
	}

	const milk::math::Vector3d& getScale() const {
		return scale_;
	}

	void setRotation(const milk::math::Vector3d& rotation) {
		rotation_ =  rotation;
		worldMatrix_.invalidate();
	}

	const milk::math::Vector3d& getRotation() const {
		return rotation_;
	}

private:

	milk::graphics::Buffer vertexBuffer_;

	milk::graphics::Buffer indexBuffer_;

	std::shared_ptr<milk::graphics::VertexShader> vertexShader_;
	
	std::shared_ptr<milk::graphics::PixelShader> pixelShader_;

	milk::math::Vector3d translation_;

	milk::math::Vector3d scale_;

	milk::math::Vector3d rotation_;

	milk::utils::Lazy<milk::math::Matrix> worldMatrix_;

	void calculateWorldTransformation(milk::math::Matrix& matrix);

};

} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MODEL_HPP_ */
