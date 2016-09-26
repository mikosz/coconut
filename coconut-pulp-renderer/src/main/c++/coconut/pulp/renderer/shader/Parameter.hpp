#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_

#include <coconut-tools/enum.hpp>

#include "coconut/milk/math/Matrix.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Scene;
class Actor;
class Material;

namespace shader {

class UnknownParameter {
public:

	CCN_MEMBER_ENUM(
		OperandType,
		(SCENE)
		(ACTOR)
		(MATERIAL)

		(MATRIX)
		(VECTOR3D)
	);

	UnknownParameter() = default;

	virtual ~UnknownParameter() noexcept = default;

	UnknownParameter(const UnknownParameter&) = delete;

	void operator=(const UnknownParameter&) = delete;

	virtual OperandType inputType() const noexcept = 0;

	virtual OperandType outputType() const noexcept = 0;

	virtual size_t size() const noexcept = 0;

	virtual bool requires16ByteAlignment() const noexcept {
		return false;
	}

protected:

	template <class T>
	struct DeducedOperandType;

	template <>
	struct DeducedOperandType<Scene> {
		static const auto type = OperandType::SCENE;
	};

	template <>
	struct DeducedOperandType<Actor> {
		static const auto type = OperandType::ACTOR;
	};

	template <>
	struct DeducedOperandType<Material> {
		static const auto type = OperandType::MATERIAL;
	};

	template <>
	struct DeducedOperandType<milk::math::Matrix> {
		static const auto type = OperandType::MATRIX;
	};

	template <>
	struct DeducedOperandType<milk::math::Vector3d> {
		static const auto type = OperandType::VECTOR3D;
	};

};

template <class... UpdateArguments>
class Parameter : public UnknownParameter {
public:

	virtual void update(void* buffer, const UpdateArguments&... data) const = 0;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_ */
