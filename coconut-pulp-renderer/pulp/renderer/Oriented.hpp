#ifndef _COCONUT_PULP_RENDERER_ORIENTED_HPP_
#define _COCONUT_PULP_RENDERER_ORIENTED_HPP_

#include <functional>

#include "milk/math/Matrix.hpp"
#include "milk/math/Vector.hpp"
#include "milk/utils/Lazy.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Oriented {
public:

	Oriented() :
		worldMatrix_(std::bind(&Oriented::calculateWorldTransformation, this, std::placeholders::_1))
	{
	}

	virtual ~Oriented() {
	}

	const milk::math::Matrix& worldTransformation() const {
		return worldMatrix_.get();
	}

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

	milk::math::Vector3d translation_;

	milk::math::Vector3d scale_;

	milk::math::Vector3d rotation_;

	milk::utils::Lazy<milk::math::Matrix> worldMatrix_;

	void calculateWorldTransformation(milk::math::Matrix& matrix) {
		matrix =
			milk::math::Matrix::rotation(rotation_.z(), rotation_.x(), rotation_.y()) *
			milk::math::Matrix::scale(scale_.x(), scale_.y(), scale_.z()) *
			milk::math::Matrix::translation(translation_.x(), translation_.y(), translation_.z())
			;
	}

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORIENTED_HPP_ */
