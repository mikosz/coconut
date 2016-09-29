#include "ParameterFactory.hpp"

#include <locale>
#include <deque>

#include <boost/algorithm/string/join.hpp>

#include <coconut-tools/logger.hpp>

#include "../Scene.hpp"
#include "../Actor.hpp"
#include "../Material.hpp"

#include "ArrayParameter.hpp"
#include "CallbackParameter.hpp"
#include "ParameterChain.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.PARAMETER_FACTORY");

using IdentifierSlices = std::deque<std::string>;

IdentifierSlices sliceIdentifier(const std::string& id) {
	IdentifierSlices result;
	
	std::locale locale;

	std::string next;
	next.reserve(id.length());

	for (auto c : id) {
		if (std::isupper(c, locale) && !next.empty()) {
			result.emplace_back(next);
			next.clear();
		} else if (c == '_' && !next.empty()) {
			result.emplace_back(next);
			next.clear();
			continue;
		}

		next.push_back(std::tolower(c, locale));
	}

	if (!next.empty()) {
		result.emplace_back(next);
	}

	CT_LOG_DEBUG << "Parameter id " << id << " sliced into [ " << result << " ]";

	return result;
}

// TODO: extract creators to external file?
using milk::math::Matrix;
using milk::math::Vector3d;

void verifyNotAnArray(const ParameterFactoryInstanceDetails& instanceDetails) {
	if (instanceDetails.arrayedElementIndex != -1) {
		throw coconut_tools::factory::error_policy::NoSuchType<ParameterFactoryInstanceDetails>(
			instanceDetails
			); // TODO: throw an error specifying that an array is used when not expected
	}
}

std::unique_ptr<Parameter<Scene>> createViewMatrixParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Matrix, Scene>>(
		[](Matrix& result, const Scene& scene) {
			result = scene.camera().viewTransformation();
		}
		);
}

std::unique_ptr<Parameter<Scene>> createProjectionMatrixParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Matrix, Scene>>(
		[](Matrix& result, const Scene& scene) {
			result = scene.lens().projectionTransformation();
		}
		);
}

std::unique_ptr<Parameter<Scene>> createEyePositionParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Vector3d, Scene>>(
		[](Vector3d& result, const Scene& scene) {
			result = scene.camera().position();
		}
		);
}

std::unique_ptr<Parameter<Actor>> createWorldMatrixParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Matrix, Actor>>(
		[](Matrix& result, const Actor& actor) {
			result = actor.worldTransformation();
		}
		);
}

std::unique_ptr<Parameter<Matrix>> createInverterParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Matrix, Matrix>>(
		[](Matrix& result, const Matrix& matrix) {
			result = matrix.inverted();
		}
		);
}

std::unique_ptr<Parameter<Matrix>> createTransposedParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Matrix, Matrix>>(
		[](Matrix& result, const Matrix& matrix) {
			result = matrix.transposed();
		}
		);
}

} // anonymous namespace

bool coconut::pulp::renderer::shader::operator==(
	const ParameterFactoryInstanceDetails& lhs, const ParameterFactoryInstanceDetails& rhs)
{
	return lhs.id == rhs.id &&
		lhs.structurePrefix == rhs.structurePrefix;
}

std::ostream& coconut::pulp::renderer::shader::operator<<(
	std::ostream& os, const ParameterFactoryInstanceDetails& instanceDetails)
{
	os
		<< instanceDetails.structurePrefix
		<< "::" << instanceDetails.id
		<< "[" << instanceDetails.arrayedElementIndex << "]";
	return os;
}

detail::ParameterCreator::ParameterCreator() {
	registerBuiltins();
}

std::unique_ptr<UnknownParameter> detail::ParameterCreator::doCreate(
	const ParameterFactoryInstanceDetails& instanceDetails
	)
{
	CT_LOG_INFO << "Creating shader parameter " << instanceDetails;

	auto slices = sliceIdentifier(instanceDetails.id);

	IdentifierSlices tail;
	ParameterFactoryInstanceDetails sliceDetails = instanceDetails;

	auto isChain = false;
	std::unique_ptr<UnknownParameter> result;

	while (!slices.empty()) {
		const auto subId = boost::join(slices, "_");
		sliceDetails.id = subId;

		if (isCreatorRegistered(sliceDetails)) {
			CT_LOG_DEBUG << "Found registered parameter for \"" << subId << "\"";
			auto next = Super::doCreate(sliceDetails, sliceDetails);
					// TODO: problem here is that we call the superclass going around the factory type
					// we could have params world and world_inv and world would be created twice
			if (!result) {
				result = std::move(next);
				sliceDetails = ParameterFactoryInstanceDetails("");
			} else {
				// TODO: could we have less of these switches?
				switch (result->inputType()) {
				case UnknownParameter::OperandType::SCENE:
					if (!isChain) {
						result = std::make_unique<ParameterChain<Scene>>(std::move(result));
					}
					dynamic_cast<ParameterChain<Scene>&>(*result).push(std::move(next));
					break;
				case UnknownParameter::OperandType::ACTOR:
					if (!isChain) {
						result = std::make_unique<ParameterChain<Actor>>(std::move(result));
					}
					dynamic_cast<ParameterChain<Actor>&>(*result).push(std::move(next));
					break;
				case UnknownParameter::OperandType::MATERIAL:
					if (!isChain) {
						result = std::make_unique<ParameterChain<Material>>(std::move(result));
					}
					dynamic_cast<ParameterChain<Material>&>(*result).push(std::move(next));
					break;
				default:
					throw BadParameterType(result->inputType());
				}

				isChain = true;
			}
			tail.swap(slices);
			tail.clear();
		} else {
			CT_LOG_DEBUG << "No parameter registered for \"" << subId << "\"";
			tail.emplace_front(std::move(slices.back()));
			slices.pop_back();
		}
	}
	
	if (!tail.empty()) {
		throw coconut_tools::factory::error_policy::NoSuchType<ParameterFactoryInstanceDetails>(instanceDetails);
			// TODO: custom exception specifying partial id?
	}

	return result;
}

void detail::ParameterCreator::registerBuiltins() {
	// SCENE
	registerCreator(ParameterFactoryInstanceDetails("view_matrix"), &createViewMatrixParameter);
	registerCreator(ParameterFactoryInstanceDetails("view"), &createViewMatrixParameter);

	registerCreator(ParameterFactoryInstanceDetails("projection_matrix"), &createProjectionMatrixParameter);
	registerCreator(ParameterFactoryInstanceDetails("projection"), &createProjectionMatrixParameter);

	registerCreator(ParameterFactoryInstanceDetails("eye_position"), &createEyePositionParameter);
	registerCreator(ParameterFactoryInstanceDetails("eye_pos"), &createEyePositionParameter);
	registerCreator(ParameterFactoryInstanceDetails("eye"), &createEyePositionParameter);

	// ACTOR
	registerCreator(ParameterFactoryInstanceDetails("world_matrix"), &createWorldMatrixParameter);
	registerCreator(ParameterFactoryInstanceDetails("world"), &createWorldMatrixParameter);

	// MATRIX
	registerCreator(ParameterFactoryInstanceDetails("inv"), &createInverterParameter);
	registerCreator(ParameterFactoryInstanceDetails("inverse"), &createInverterParameter);

	registerCreator(ParameterFactoryInstanceDetails("transpose"), &createTransposedParameter);
}
