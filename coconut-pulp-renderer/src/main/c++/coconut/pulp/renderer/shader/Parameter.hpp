#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_

 #include <coconut/milk/math/Matrix.hpp>

namespace coconut {
namespace pulp {
namespace renderer {

class Scene;
class Actor;
class Material;

namespace shader {

class UnknownParameter {
public:

	enum class OperandType {
		SCENE,
		ACTOR,
		MATERIAL,

		MATRIX,
		VECTOR3D,
	};

	virtual ~UnknownParameter() noexcept = default;

	virtual OperandType inputType() const noexcept = 0;

	virtual OperandType outputType() const noexcept = 0;

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

};

template <class... UpdateArguments>
class Parameter : public UnknownParameter {
public:

	Parameter(size_t size) :
		size_(size)
	{
	}

	Parameter(const Parameter&) = delete;

	void operator=(const Parameter&) = delete;

	virtual void update(void* buffer, const UpdateArguments&... data) = 0;

	virtual bool requires16ByteAlignment() const {
		return false;
	}

	size_t size() const {
		return size_;
	}

private:

	size_t size_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_ */
