#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_

#include <memory>

#include <coconut-tools/enum.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/math/Matrix.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Scene;
class Actor;
class Material;

namespace shader {

class Parameter {
public:

	CCN_MEMBER_ENUM(
		OperandType,
		(SCENE)
		(ACTOR)
		(MATERIAL)

		(MATRIX)
		(VECTOR3D)
		(UINT32)

		(OBJECT)
	);

	template <class T>
	struct DeducedOperandType;

	template <class T>
	struct DeducedOperandType<T*> {
		static const auto type = OperandType::OBJECT;
	};

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

	template <>
	struct DeducedOperandType<std::uint32_t> {
		static const auto type = OperandType::UINT32;
	};

	Parameter() = default;

	virtual ~Parameter() noexcept = default;

	Parameter(const Parameter&) = delete;

	void operator=(const Parameter&) = delete;

	virtual OperandType inputType() const noexcept = 0;

	size_t size() const noexcept;

	virtual bool requires16ByteAlignment() const noexcept {
		return false;
	}

	void update(void* output, const void* input) const; // TODO: accept two std::vector<uint8> buffers to be able to check sizes and reuse memory?

	OperandType outputType() const noexcept;

	void setNext(std::shared_ptr<Parameter> next); // TODO: put this in a subclass? StrucutredParameter doesn't want it

protected:

	virtual void updateThis(void* output, const void* input) const = 0;

	virtual OperandType thisOutputType() const noexcept = 0;

	virtual size_t thisSize() const noexcept = 0;

private:

	std::shared_ptr<Parameter> next_;

};

class IncompatibleParameters : public coconut_tools::exceptions::RuntimeError {
public:

	IncompatibleParameters(const Parameter& parameter, const Parameter& next) :
		coconut_tools::exceptions::RuntimeError(
			"Incopatible parameters - original parameters output type is " +
			toString(parameter.outputType()) +
			", next parameter input type is " +
			toString(next.inputType())
			),
		parameterOutput_(parameter.outputType()),
		nextInput_(next.inputType())
	{
	}

	Parameter::OperandType output() const noexcept {
		return parameterOutput_;
	}

	Parameter::OperandType input() const noexcept {
		return nextInput_;
	}
	
private:

	Parameter::OperandType parameterOutput_;

	Parameter::OperandType nextInput_;
	
};

template <class InputType, class OutputType>
class ConcreteParameter : public Parameter {
public:

	OperandType inputType() const noexcept override final {
		return typename DeducedOperandType<InputType>::type;
	}

protected:

	virtual void updateThis(OutputType& output, const InputType& input) const = 0;

	void updateThis(void* output, const void* input) const override final {
		const auto& concreteInput = *reinterpret_cast<const InputType*>(input);
		auto& concreteOutput = *reinterpret_cast<OutputType*>(output);

		updateThis(concreteOutput, concreteInput);
	}

	OperandType thisOutputType() const noexcept override final {
		return typename DeducedOperandType<OutputType>::type;
	}

	size_t thisSize() const noexcept override final {
		return sizeof(OutputType); // TODO: accept offset and size as constructor parameters (wont need size() and requires16ByteAlignment())
	}

};

} // namespace shader

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_ */
