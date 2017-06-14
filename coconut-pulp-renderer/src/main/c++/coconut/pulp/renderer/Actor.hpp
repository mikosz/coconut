#ifndef _COCONUT_PULP_RENDERER_ACTOR_HPP_
#define _COCONUT_PULP_RENDERER_ACTOR_HPP_

#include <chrono>
#include <string>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"
#include "coconut/pulp/renderer/shader/Property.hpp"

#include "Orientation.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class CommandBuffer;

class Actor { // TODO: api, is this needed at all?
public:

	Actor(std::string modelId) :
		modelId_(std::move(modelId))
	{
	}

	virtual void update(std::chrono::milliseconds dt) {
	}

	virtual void bindShaderProperties(
		renderer::shader::Properties& properties,
		std::string objectId
		) const
	{
	}

	// TODO: move below functions to interface?
	const Matrix4x4& worldTransformation() const {
		return orientation_.worldTransformation();
	}

	void setTranslation(const Vec3& translation) {
		orientation_.setTranslation(translation);
	}

	const Vec3& getTranslation() const {
		return orientation_.getTranslation();
	}

	void setScale(const Vec3& scale) {
		orientation_.setScale(scale); // TODO: block non-uniform scale?
	}

	const Vec3& getScale() const {
		return orientation_.getScale();
	}

	void setRotation(const Vec3& rotation) {
		orientation_.setRotation(rotation);
	}

	const Vec3& getRotation() const {
		return orientation_.getRotation();
	}

	const std::string& modelId() const {
		return modelId_;
	}

private:

	std::string modelId_;

	Orientation orientation_;

};

CT_MAKE_POINTER_DEFINITIONS(Actor);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ACTOR_HPP_ */
