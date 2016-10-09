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
		(VECTOR4D)
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
	struct DeducedOperandType<milk::math::Vector4d> {
		static const auto type = OperandType::VECTOR4D;
	};

	template <>
	struct DeducedOperandType<std::uint32_t> {
		static const auto type = OperandType::UINT32;
	};

	Parameter(size_t arrayElements = 0) :
		arrayElements_(arrayElements)
	{
	}

	virtual ~Parameter() noexcept = default;

	Parameter(const Parameter&) = delete;

	void operator=(const Parameter&) = delete;

	virtual OperandType inputType() const noexcept = 0;

	size_t size() const noexcept; // TODO: doesn't work for structured parameters

	virtual bool requires16ByteAlignment() const noexcept {
		return false;
	}

	virtual void* update(void* output, const void* input) const; // TODO: accept a smarter buffer pointer, to know the size

	OperandType outputType() const noexcept; // TODO: doesn't work for structured parameters

	void setNext(std::shared_ptr<Parameter> next); // TODO: put this in a subclass? StrucutredParameter doesn't want it
		// OR drop StructuredParameter altogether and replace setNext with addNext?

protected:

	virtual void* updateThis(void* output, const void* input, size_t arrayIndex) const = 0;

	virtual OperandType thisOutputType() const noexcept = 0;

	virtual size_t thisSize() const noexcept = 0;

	Parameter* getNext() {
		return next_.get();
	}

	const Parameter* getNext() const {
		return next_.get();
	}

private:

	std::shared_ptr<Parameter> next_;

	size_t arrayElements_;

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

	ConcreteParameter(size_t arrayElements = 0) :
		Parameter(arrayElements)
	{
	}

	OperandType inputType() const noexcept override final {
		return typename DeducedOperandType<InputType>::type;
	}

protected:

	virtual void updateThis(OutputType& output, const InputType& input, size_t arrayIndex) const = 0;

	void* updateThis(void* output, const void* input, size_t arrayIndex) const override final {
		const auto& concreteInput = *reinterpret_cast<const InputType*>(input);
		auto& concreteOutput = *reinterpret_cast<OutputType*>(output);

		updateThis(concreteOutput, concreteInput, arrayIndex);

		return reinterpret_cast<std::uint8_t*>(output) + thisSize();
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
