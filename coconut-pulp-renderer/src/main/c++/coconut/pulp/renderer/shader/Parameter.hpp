#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_

#include <memory>

#include <coconut-tools/enum.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/pulp/math/Matrix.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

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
		static const auto size = sizeof(T*);

		static void store(void* buffer, T* data) {
			auto*& storedPtr = reinterpret_cast<T*&>(buffer);
			storedPtr = buffer;
		}
	};

	template <>
	struct DeducedOperandType<Scene> {
		static const auto type = OperandType::SCENE;
	};

	template <>
	struct DeducedOperandType<std::vector<const Actor*>> {
		static const auto type = OperandType::ACTOR;
	};

	template <>
	struct DeducedOperandType<Material> {
		static const auto type = OperandType::MATERIAL;
	};

	template <>
	struct DeducedOperandType<Matrix4x4> {
		static const auto type = OperandType::MATRIX;
		static const auto size = sizeof(float) * 16;

		static void store(void* buffer, const Matrix4x4& matrix) {
			std::memcpy(buffer, &matrix, size); // TODO: can't assume that Matrix is 1:1 represented in buffer
		}
	};

	template <>
	struct DeducedOperandType<Vec3> {
		static const auto type = OperandType::VECTOR3D;
		static const auto size = sizeof(float) * 3;

		static void store(void* buffer, const Vec3& vector) {
			std::memcpy(buffer, &vector, size); // TODO: TEMP!?
		}
	};

	template <>
	struct DeducedOperandType<Vec4> {
		static const auto type = OperandType::VECTOR4D;
		static const auto size = sizeof(float) * 4;

		static void store(void* buffer, const Vec4& vector) {
			std::memcpy(buffer, &vector, size); // TODO: TEMP!?
		}
	};

	template <>
	struct DeducedOperandType<std::uint32_t> {
		static const auto type = OperandType::UINT32;
		static const auto size = 4;

		static void store(void* buffer, std::uint32_t ui) {
			auto* stored = reinterpret_cast<std::uint32_t*>(buffer); // TODO: messy
			*stored = ui;
		}
	};

	Parameter(size_t offset, size_t arrayElements = 0, size_t arrayElementOffset = 0) :
		offset_(offset),
		arrayElements_(arrayElements),
		arrayElementOffset_(arrayElementOffset)
	{
	}

	virtual ~Parameter() noexcept = default;

	Parameter(const Parameter&) = delete;

	void operator=(const Parameter&) = delete;

	virtual OperandType inputType() const noexcept = 0;

	virtual bool requires16ByteAlignment() const noexcept {
		return false;
	}

	virtual void update(void* output, const void* input) const; // TODO: accept a smarter buffer pointer, to know the size

	OperandType outputType() const noexcept; // TODO: doesn't work for structured parameters

	void setNext(std::shared_ptr<Parameter> next); // TODO: put this in a subclass? StrucutredParameter doesn't want it
		// OR drop StructuredParameter altogether and replace setNext with addNext?

	size_t size() const noexcept;

protected:

	virtual void updateThis(void* output, const void* input, size_t arrayIndex) const = 0;

	virtual size_t thisSize() const noexcept = 0;

	virtual OperandType thisOutputType() const noexcept = 0;

	Parameter* getNext() {
		return next_.get();
	}

	const Parameter* getNext() const {
		return next_.get();
	}

private:

	size_t offset_;

	size_t arrayElementOffset_;

	size_t arrayElements_;

	std::shared_ptr<Parameter> next_;

};

CCN_MAKE_POINTER_DEFINITIONS(Parameter);

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

	ConcreteParameter(size_t padding, size_t arrayElements = 0) :
		Parameter(padding, arrayElements)
	{
	}

	OperandType inputType() const noexcept override final {
		return typename DeducedOperandType<InputType>::type;
	}

protected:

	virtual void updateThis(OutputType& output, const InputType& input, size_t arrayIndex) const = 0;

	void updateThis(void* output, const void* input, size_t arrayIndex) const override final {
		const auto& concreteInput = *reinterpret_cast<const InputType*>(input);
		
		OutputType concreteOutput;
		updateThis(concreteOutput, concreteInput, arrayIndex);

		DeducedOperandType<OutputType>::store(output, concreteOutput);
	}

	OperandType thisOutputType() const noexcept override final {
		return typename DeducedOperandType<OutputType>::type;
	}

	size_t thisSize() const noexcept override final {
		return DeducedOperandType<OutputType>::size;
	}

};

} // namespace shader

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_ */
