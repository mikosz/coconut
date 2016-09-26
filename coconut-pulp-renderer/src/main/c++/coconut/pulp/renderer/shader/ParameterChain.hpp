#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETERCHAIN_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETERCHAIN_HPP_

#include <algorithm>
#include <vector>

#include <coconut-tools/exceptions/LogicError.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class IncompatibleChainParameters : public coconut_tools::exceptions::RuntimeError {
public:

	IncompatibleChainParameters(UnknownParameter::OperandType output, UnknownParameter::OperandType input) :
		coconut_tools::exceptions::RuntimeError("Incopatible parameter modifiers in parameter chain - "
			"output type " + toString(output) + " used with input" + toString(input)
			),
		output_(output),
		input_(input)
	{
	}

	UnknownParameter::OperandType output() const noexcept {
		return output_;
	}

	UnknownParameter::OperandType input() const noexcept {
		return input_;
	}
	
private:

	UnknownParameter::OperandType output_;

	UnknownParameter::OperandType input_;
	
};

class BadParameterModifierType : public coconut_tools::exceptions::RuntimeError {
public:

	BadParameterModifierType(UnknownParameter::OperandType type) :
		coconut_tools::exceptions::RuntimeError("Provided type: " + toString(type) + " cannot be used as a parameter modifier"),
		type_(type)
	{
	}

	UnknownParameter::OperandType type() const noexcept {
		return type_;
	}

private:

	UnknownParameter::OperandType type_;

};

template <class... UpdateArguments>
class ParameterChain : public Parameter<UpdateArguments...> {
public:

	ParameterChain(std::shared_ptr<UnknownParameter> initialParameter) {
		push(initialParameter);
	}

	void update(void* buffer, const UpdateArguments&... data) const override {
		using milk::math::Matrix;
		using milk::math::Vector3d;

		auto parameterIt = parameters_.begin();
		auto parametersEnd = parameters_.end();

		doUpdate(**parameterIt, data...);

		while (parameterIt != parametersEnd) {
			resultBuffer_.swap(argumentBuffer_);

			// TODO: could we have less of these switches?
			switch ((*parameterIt)->inputType()) {
			case OperandType::MATRIX: {
				const auto& matrix = *reinterpret_cast<const Matrix*>(argumentBuffer_.data());
				doUpdate(**parameterIt, matrix);
				break;
			}
			case OperandType::VECTOR3D: {
				const auto& vector = *reinterpret_cast<const Vector3d*>(argumentBuffer_.data());
				doUpdate(**parameterIt, vector);
				break;
			}
			default:
				throw coconut_tools::exceptions::LogicError("Unexpected parameter input type: " + toString((*parameterIt)->inputType()));
			}

			++parameterIt;
		}

		std::copy(resultBuffer_.begin(), resultBuffer_.end(), reinterpret_cast<std::uint8_t*>(buffer));
	}

	bool requires16ByteAlignment() const noexcept override {
		return parameters_.front()->requires16ByteAlignment();
	}

	size_t size() const noexcept override {
		return parameters_.back()->size();
	}

	OperandType inputType() const noexcept override {
		return parameters_.front()->inputType();
	}

	OperandType outputType() const noexcept override {
		return parameters_.back()->outputType();
	}

	void push(std::shared_ptr<UnknownParameter> parameter) {
		if (!parameters_.empty() && parameter->inputType() != parameters_.back()->outputType()) {
			throw IncompatibleChainParameters(parameters_.back()->outputType(), parameter->inputType());
		}
		
		parameters_.emplace_back(parameter);

		const auto maxSize = std::max(parameter->size(), argumentBuffer_.size());
		argumentBuffer_.resize(maxSize);
		resultBuffer_.resize(maxSize);
	}

private:

	using Element = std::shared_ptr<UnknownParameter>;

	std::vector<Element> parameters_;

	mutable std::vector<std::uint8_t> argumentBuffer_;

	mutable std::vector<std::uint8_t> resultBuffer_;

	void doUpdate(UnknownParameter& parameter, const Scene& scene) const {
		dynamic_cast<Parameter<Scene>&>(parameter).update(resultBuffer_.data(), scene);
	}

	void doUpdate(UnknownParameter& parameter, const Actor& actor) const {
		dynamic_cast<Parameter<Actor>&>(parameter).update(resultBuffer_.data(), actor);
	}

	void doUpdate(UnknownParameter& parameter, const Material& material) const {
		dynamic_cast<Parameter<Material>&>(parameter).update(resultBuffer_.data(), material);
	}

	void doUpdate(UnknownParameter& parameter, const milk::math::Matrix& matrix) const {
		dynamic_cast<Parameter<milk::math::Matrix>&>(parameter).update(resultBuffer_.data(), matrix);
	}

	void doUpdate(UnknownParameter& parameter, const milk::math::Vector3d& vector) const {
		dynamic_cast<Parameter<milk::math::Vector3d>&>(parameter).update(resultBuffer_.data(), vector);
	}

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETERCHAIN_HPP_ */
