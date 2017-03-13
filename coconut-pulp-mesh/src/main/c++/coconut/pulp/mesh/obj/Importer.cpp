#include "Importer.hpp"

#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <tuple>

#include <coconut-tools/logger.hpp>
#include <coconut-tools/utils/Range.hpp>
#include <coconut-tools/utils/hash-combine.hpp>

#include "coconut/pulp/mesh/MaterialConfiguration.hpp"
#include "Parser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::mesh;
using namespace coconut::pulp::mesh::obj;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.MESH.OBJ.IMPORTER");

using VertexDescriptor = std::tuple<size_t, size_t, size_t>;

MaterialConfiguration createMaterialConfiguration(
	const Parser::Materials::value_type& materialDataEntry,
	const milk::graphics::Sampler::Configuration& samplerConfiguration,
	const milk::graphics::RenderState::Configuration& renderStateConfiguration,
	const milk::FilesystemContext& filesystemContext
	)
{
	const auto& materialData = materialDataEntry.second;

	auto material = MaterialConfiguration();

	material.shaderName() = "mesh"; // TODO: literal in code, export

	material.passType() =  (materialData.diffuseColour.a() < 0.999f) ?
		MaterialConfiguration::PassType::TRANSPARENT :
		MaterialConfiguration::PassType::OPAQUE;

	material.renderStateConfiguration() = renderStateConfiguration;

	auto& properties = material.properties();
	properties.emplace(MaterialConfiguration::AMBIENT_COLOUR_PROPERTY, materialData.ambientColour);
	properties.emplace(MaterialConfiguration::DIFFUSE_COLOUR_PROPERTY, materialData.diffuseColour);
	properties.emplace(MaterialConfiguration::SPECULAR_COLOUR_PROPERTY, materialData.specularColour);
	properties.emplace(MaterialConfiguration::SPECULAR_EXPONENT_PROPERTY, materialData.specularExponent);

	if (!materialData.diffuseMap.empty()) {
		material.addTexture(
			MaterialConfiguration::DIFFUSE_MAP_TEXTURE,
			filesystemContext.currentWorkingDirectory() / materialData.diffuseMap,
			samplerConfiguration
			);
	}

	return material;
}

struct VertexDescriptorHasher { // TODO: add tuple hash template to hash_combine?

	size_t operator()(const VertexDescriptor& vertexDescriptor) const {
		auto seed = static_cast<size_t>(0);
		seed = coconut_tools::utils::hashCombine(seed, std::get<0>(vertexDescriptor));
		seed = coconut_tools::utils::hashCombine(seed, std::get<1>(vertexDescriptor));
		seed = coconut_tools::utils::hashCombine(seed, std::get<2>(vertexDescriptor));
		return seed;
	}

};

void generateNormals(
	Submesh::Vertices& vertices,
	const Submesh::Indices& indices,
	milk::graphics::PrimitiveTopology primitiveTopology)
{
	CT_LOG_DEBUG << "Generating normals";

	// TODO is it possible to have other topologies in .obj? if not, drop parameter
	assert(primitiveTopology == milk::graphics::PrimitiveTopology::TRIANGLE_LIST);

	auto affectingFaces = std::unordered_multimap<size_t, size_t>();

	for (const auto faceIdx : coconut_tools::range<size_t>(0, indices.size(), 3)) {
		for (const auto vertexIdx : coconut_tools::range(faceIdx, faceIdx + 3)) {
			affectingFaces.emplace(vertexIdx, faceIdx);
		}
	}

	auto faceNormals = std::unordered_map<size_t, Vector>();

	for (const auto vertexIdx : coconut_tools::range<size_t>(0, vertices.size())) {
		auto& vertex = vertices[vertexIdx];
		auto range = affectingFaces.equal_range(vertexIdx);

		vertex.normal = Vector(0.0f, 0.0f, 0.0f);

		CT_LOG_TRACE << "Calculating normal for vertex " << vertexIdx << ": " << vertex.position;

		while (range.first != range.second) {
			const auto faceIdx = range.first->second;
			auto faceNormalIt = faceNormals.find(faceIdx);
			if (faceNormalIt == faceNormals.end()) {
				const auto a = vertices[indices[faceIdx + 1]].position - vertices[indices[faceIdx]].position;
				const auto b = vertices[indices[faceIdx + 2]].position - vertices[indices[faceIdx]].position;
				const auto cross = a.cross(b);

				CT_LOG_TRACE << "Face vectors: " << a << " x " << b << " = " << cross;

				faceNormalIt = faceNormals.emplace_hint(faceNormalIt, faceIdx, cross);
			}

			vertex.normal += faceNormalIt->second;

			CT_LOG_TRACE << "Temporary normal with face " << (faceIdx / 3) << " is " << vertex.normal;

			++range.first;
		}

		vertex.normal.normalise();

		CT_LOG_TRACE << "Final normal is " << vertex.normal;
	}
}

} // anonymous namespace

Mesh obj::Importer::import(
	const milk::fs::RawData& data,
	const milk::FilesystemContext& filesystemContext
	)
{
	Parser parser;
	parser.parse(data, filesystemContext);
	
	auto materials = Mesh::MaterialConfigurations();

	auto renderStateConfiguration = milk::graphics::RenderState::Configuration();
	renderStateConfiguration.cullMode = milk::graphics::RenderState::CullMode::BACK;
	renderStateConfiguration.fillMode = milk::graphics::RenderState::FillMode::SOLID;
	renderStateConfiguration.frontCounterClockwise = false;

	auto samplerConfiguration = milk::graphics::Sampler::Configuration();
	samplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR; // TODO

	for (const auto& parsedMaterialConfiguration : parser.materials()) {
		materials.emplace(
			parsedMaterialConfiguration.first,
			createMaterialConfiguration(parsedMaterialConfiguration, samplerConfiguration, renderStateConfiguration, filesystemContext)
			);
	}

	auto hasFaces = false;

	const auto& positionData = parser.positions();
	const auto& textureCoordinateData = parser.textureCoordinateIndex();
	const auto& normalData = parser.normals();
	auto submeshes = Mesh::Submeshes();

	for (const auto& object : parser.objects()) {
		for (auto group : object.groups) {
			if (!group.faces.empty()) {
				hasFaces = true;
				
				auto normalsNeedGeneration = false;
				auto vertices = Submesh::Vertices();
				auto indices = Submesh::Indices();

				auto vertexIndices = std::unordered_map<VertexDescriptor, size_t, VertexDescriptorHasher>();

				for (const auto& face : group.faces) {
					for (const auto& vertexData : face.vertices) {
						const auto vertexDesc = VertexDescriptor(
							vertexData.positionIndex, vertexData.textureCoordinateIndex, vertexData.normalIndex);
						if (vertexIndices.count(vertexDesc) == 0) {
							vertices.emplace_back();
							
							auto& vertex = vertices.back();
							vertex.position = positionData[vertexData.positionIndex];
							vertex.textureCoordinate = textureCoordinateData[vertexData.textureCoordinateIndex];
							if (vertexData.normalIndex == Parser::NORMAL_INDEX_UNKNOWN) {
								normalsNeedGeneration = true;
							} else {
								vertex.normal = normalData[vertexData.normalIndex];
							}

							vertexIndices.insert(std::make_pair(vertexDesc, vertices.size() - 1));
						}

						indices.emplace_back(vertexIndices[vertexDesc]);
					}
				}

				if (normalsNeedGeneration) {
					generateNormals(vertices, indices, milk::graphics::PrimitiveTopology::TRIANGLE_LIST);
				}

				submeshes.emplace_back(
					std::move(vertices),
					std::move(indices),
					group.material,
					milk::graphics::PrimitiveTopology::TRIANGLE_LIST
					);
			}
		}
	}

	if (!hasFaces) {
		throw std::runtime_error("Model has no faces");
	}

	return Mesh(std::move(submeshes), std::move(materials));
}
