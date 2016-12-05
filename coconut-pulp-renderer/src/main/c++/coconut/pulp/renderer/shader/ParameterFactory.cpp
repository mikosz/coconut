#include "ParameterFactory.hpp"

#include <deque>

#include <boost/algorithm/string/join.hpp>

#include <coconut-tools/logger.hpp>

#include "coconut/milk/utils/sliceIdentifier.hpp"

#include "../lighting/DirectionalLight.hpp"
#include "../lighting/PointLight.hpp"
#include "../Scene.hpp"
#include "../Actor.hpp"
#include "../PhongMaterial.hpp"

#include "CallbackParameter.hpp"
#include "StructuredParameter.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.PARAMETER_FACTORY");

// TODO: extract creators to external file?
using milk::math::Matrix;
using milk::math::Vector3d;
using milk::math::Vector4d;

void verifyNotAnArray(const ParameterFactoryInstanceDetails& instanceDetails) {
	if (instanceDetails.arraySize != 0) {
		throw coconut_tools::factory::error_policy::NoSuchType<ParameterFactoryInstanceDetails>(
			instanceDetails
			); // TODO: throw an error specifying that an array is used when not expected
	}
}

void verifyIsArray(const ParameterFactoryInstanceDetails& instanceDetails) {
	if (instanceDetails.arraySize == 0) {
		throw coconut_tools::factory::error_policy::NoSuchType<ParameterFactoryInstanceDetails>(
			instanceDetails
			); // TODO: throw an error specifying that an array is expected
	}
}

std::unique_ptr<Parameter> createViewMatrixParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Scene, Matrix>>(
		[](Matrix& result, const Scene& scene, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = scene.camera().viewTransformation();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createProjectionMatrixParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Scene, Matrix>>(
		[](Matrix& result, const Scene& scene, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = scene.lens().projectionTransformation();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createEyePositionParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Scene, Vector3d>>(
		[](Vector3d& result, const Scene& scene, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = scene.camera().position();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createDirectionalLightsParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyIsArray(instanceDetails);

	return std::make_unique<StructuredParameter>(
		[](const void* scenePtr, size_t index) -> const lighting::DirectionalLight* {
			const auto& scene = *reinterpret_cast<const Scene*>(scenePtr);

			if (index < scene.directionalLights().size()) {
				return &scene.directionalLights()[index];
			} else {
				return nullptr;
			}
		},
		Parameter::OperandType::SCENE,
		instanceDetails.padding,
		instanceDetails.arraySize,
		instanceDetails.arrayElementOffset
		);
}

std::unique_ptr<Parameter> createDirectionalLightsCountParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Scene, std::uint32_t>>(
		[](std::uint32_t& result, const Scene& scene, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = static_cast<std::uint32_t>(scene.directionalLights().size());
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createPointLightsParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyIsArray(instanceDetails);

	return std::make_unique<StructuredParameter>(
		[](const void* scenePtr, size_t index) -> const lighting::PointLight* {
			const auto& scene = *reinterpret_cast<const Scene*>(scenePtr);

			if (index < scene.pointLights().size()) {
				return &scene.pointLights()[index];
			} else {
				return nullptr;
			}
		},
		Parameter::OperandType::SCENE,
		instanceDetails.padding,
		instanceDetails.arraySize,
		instanceDetails.arrayElementOffset
		);
}

std::unique_ptr<Parameter> createPointLightsCountParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Scene, std::uint32_t>>(
		[](std::uint32_t& result, const Scene& scene, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = static_cast<std::uint32_t>(scene.pointLights().size());
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createWorldMatrixParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Actor, Matrix>>(
		[](Matrix& result, const Actor& actor, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = actor.worldTransformation();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createPhongMaterialParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<StructuredParameter>(
		[](const void* materialPtr, size_t index) {
			assert(index == 0);

			const auto& material = *reinterpret_cast<const Material*>(materialPtr);

			return &dynamic_cast<const PhongMaterial&>(material);
		},
		Parameter::OperandType::MATERIAL,
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createInverterParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Matrix, Matrix>>(
		[](Matrix& result, const Matrix& matrix, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = matrix.inverted();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createTransposedParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<Matrix, Matrix>>(
		[](Matrix& result, const Matrix& matrix, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = matrix.transposed();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createDirectionalLightAmbientColourParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<lighting::DirectionalLight*, Vector4d>>(
		[](Vector4d& result, const lighting::DirectionalLight* light, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = light->ambientColour();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createDirectionalLightDiffuseColourParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<lighting::DirectionalLight*, Vector4d>>(
		[](Vector4d& result, const lighting::DirectionalLight* light, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = light->diffuseColour();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createDirectionalLightSpecularColourParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<lighting::DirectionalLight*, Vector4d>>(
		[](Vector4d& result, const lighting::DirectionalLight* light, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = light->specularColour();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createDirectionalLightDirectionParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<lighting::DirectionalLight*, Vector3d>>(
		[](Vector3d& result, const lighting::DirectionalLight* light, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = light->direction();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createPointLightAmbientColourParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<lighting::PointLight*, Vector4d>>(
		[](Vector4d& result, const lighting::PointLight* light, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = light->ambientColour();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createPointLightDiffuseColourParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<lighting::PointLight*, Vector4d>>(
		[](Vector4d& result, const lighting::PointLight* light, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = light->diffuseColour();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createPointLightSpecularColourParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<lighting::PointLight*, Vector4d>>(
		[](Vector4d& result, const lighting::PointLight* light, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = light->specularColour();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createPointLightPositionParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<lighting::PointLight*, Vector3d>>(
		[](Vector3d& result, const lighting::PointLight* light, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = light->position();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createPointLightAttenuationParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<lighting::PointLight*, Vector3d>>(
		[](Vector3d& result, const lighting::PointLight* light, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = light->attenuation();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createPhongMaterialAmbientColourParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<PhongMaterial*, Vector4d>>(
		[](Vector4d& result, const PhongMaterial* material, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = material->ambientColour();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createPhongMaterialDiffuseColourParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<PhongMaterial*, Vector4d>>(
		[](Vector4d& result, const PhongMaterial* material, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = material->diffuseColour();
		},
		instanceDetails.padding
		);
}

std::unique_ptr<Parameter> createPhongMaterialSpecularColourParameter(const ParameterFactoryInstanceDetails& instanceDetails) {
	verifyNotAnArray(instanceDetails);

	return std::make_unique<CallbackParameter<PhongMaterial*, Vector4d>>(
		[](Vector4d& result, const PhongMaterial* material, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = material->specularColour();
		},
		instanceDetails.padding
		);
}

} // anonymous namespace

bool coconut::pulp::renderer::shader::operator==(
	const ParameterFactoryInstanceDetails& lhs, const ParameterFactoryInstanceDetails& rhs)
{
#pragma message("TODO: figure out what should be compared here, possibly change operator== to some other name if possible")
	return
		lhs.id == rhs.id &&
		lhs.parentType == rhs.parentType &&
		lhs.padding == rhs.padding /* &&
		lhs.arrayElementOffset == rhs.arrayElementOffset */
		;
}

std::ostream& coconut::pulp::renderer::shader::operator<<(
	std::ostream& os, const ParameterFactoryInstanceDetails& instanceDetails)
{
	os << instanceDetails.parentType << "::" << instanceDetails.id;
	if (instanceDetails.arraySize != 0) {
		os << "[" << instanceDetails.arraySize << "]";
	}
	os << " at offset: " << instanceDetails.padding;
	if (instanceDetails.arraySize != 0) {
		os << ", array element offset: " << instanceDetails.arrayElementOffset;
	}
	return os;
}

detail::ParameterCreator::ParameterCreator() {
	registerBuiltins();
}

std::unique_ptr<Parameter> detail::ParameterCreator::doCreate(
	const ParameterFactoryInstanceDetails& instanceDetails
	)
{
	CT_LOG_INFO << "Creating shader parameter " << instanceDetails;

	auto slices = milk::utils::sliceIdentifier<std::deque>(instanceDetails.id);
	CT_LOG_DEBUG << "Parameter id " << instanceDetails.id << " sliced into [ " << slices << " ]";

	decltype(slices) tail;
	ParameterFactoryInstanceDetails sliceDetails = instanceDetails;

	std::unique_ptr<Parameter> result;
	Parameter* last = nullptr;

	while (!slices.empty()) {
		const auto subId = boost::join(slices, "_");
		sliceDetails.id = subId;

		auto sliceDetailsNoPadding = sliceDetails;
		sliceDetailsNoPadding.padding = 0;

		if (isCreatorRegistered(sliceDetailsNoPadding)) {
			CT_LOG_DEBUG << "Found registered parameter for \"" << subId << "\"";
			auto next = Super::doCreate(sliceDetailsNoPadding, sliceDetails);
					// TODO: problem here is that we call the superclass going around the factory type
					// we could have params world and world_inv and world would be created twice
			if (!result) {
				result = std::move(next);
				last = result.get();
				sliceDetails = ParameterFactoryInstanceDetails("");
			} else {
				auto* newLast = next.get();
				last->setNext(std::move(next));
				last = newLast;
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

	registerCreator(ParameterFactoryInstanceDetails("directional_lights"), &createDirectionalLightsParameter);
	registerCreator(ParameterFactoryInstanceDetails("directional_lights_count"), &createDirectionalLightsCountParameter);

	registerCreator(ParameterFactoryInstanceDetails("point_lights"), &createPointLightsParameter);
	registerCreator(ParameterFactoryInstanceDetails("point_lights_count"), &createPointLightsCountParameter);

	// ACTOR
	registerCreator(ParameterFactoryInstanceDetails("world_matrix"), &createWorldMatrixParameter);
	registerCreator(ParameterFactoryInstanceDetails("world"), &createWorldMatrixParameter);

	// MATERIAL
	registerCreator(ParameterFactoryInstanceDetails("phong_material"), &createPhongMaterialParameter);

	// MATRIX
	registerCreator(ParameterFactoryInstanceDetails("inv"), &createInverterParameter);
	registerCreator(ParameterFactoryInstanceDetails("inverse"), &createInverterParameter);

	registerCreator(ParameterFactoryInstanceDetails("transpose"), &createTransposedParameter);

	// DIRECTIONAL LIGHT
	// TODO: group types with similar interface perhaps?
	// TODO: create a special parameter type that would facilitate calling getters?
	// TODO: have a EXPOSE_SHADER_PARAMETER macro that somehow facilitates all this shit?
	registerCreator(ParameterFactoryInstanceDetails("ambient_colour", "DirectionalLight"), &createDirectionalLightAmbientColourParameter);
	registerCreator(ParameterFactoryInstanceDetails("ambient", "DirectionalLight"), &createDirectionalLightAmbientColourParameter);

	registerCreator(ParameterFactoryInstanceDetails("diffuse_colour", "DirectionalLight"), &createDirectionalLightDiffuseColourParameter);
	registerCreator(ParameterFactoryInstanceDetails("diffuse", "DirectionalLight"), &createDirectionalLightDiffuseColourParameter);

	registerCreator(ParameterFactoryInstanceDetails("specular_colour", "DirectionalLight"), &createDirectionalLightSpecularColourParameter);
	registerCreator(ParameterFactoryInstanceDetails("specular", "DirectionalLight"), &createDirectionalLightSpecularColourParameter);

	registerCreator(ParameterFactoryInstanceDetails("direction", "DirectionalLight"), &createDirectionalLightDirectionParameter);

	// POINT LIGHT
	registerCreator(ParameterFactoryInstanceDetails("ambient_colour", "PointLight"), &createPointLightAmbientColourParameter);
	registerCreator(ParameterFactoryInstanceDetails("ambient", "PointLight"), &createPointLightAmbientColourParameter);

	registerCreator(ParameterFactoryInstanceDetails("diffuse_colour", "PointLight"), &createPointLightDiffuseColourParameter);
	registerCreator(ParameterFactoryInstanceDetails("diffuse", "PointLight"), &createPointLightDiffuseColourParameter);

	registerCreator(ParameterFactoryInstanceDetails("specular_colour", "PointLight"), &createPointLightSpecularColourParameter);
	registerCreator(ParameterFactoryInstanceDetails("specular", "PointLight"), &createPointLightSpecularColourParameter);

	registerCreator(ParameterFactoryInstanceDetails("position", "PointLight"), &createPointLightPositionParameter);

	registerCreator(ParameterFactoryInstanceDetails("attenuation", "PointLight"), &createPointLightAttenuationParameter);

	// PHONG MATERIAL
	registerCreator(ParameterFactoryInstanceDetails("ambient_colour", "PhongMaterial"), &createPhongMaterialAmbientColourParameter);
	registerCreator(ParameterFactoryInstanceDetails("ambient", "PhongMaterial"), &createPhongMaterialAmbientColourParameter);

	registerCreator(ParameterFactoryInstanceDetails("diffuse_colour", "PhongMaterial"), &createPhongMaterialDiffuseColourParameter);
	registerCreator(ParameterFactoryInstanceDetails("diffuse", "PhongMaterial"), &createPhongMaterialDiffuseColourParameter);

	registerCreator(ParameterFactoryInstanceDetails("specular_colour", "PhongMaterial"), &createPhongMaterialSpecularColourParameter);
	registerCreator(ParameterFactoryInstanceDetails("specular", "PhongMaterial"), &createPhongMaterialSpecularColourParameter);
}
