#include "Importer.hpp"

#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <tuple>

#include <coconut-tools/utils/hash-combine.hpp>

#include "Parser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::mesh;
using namespace coconut::pulp::mesh::obj;

namespace /* anonymous */ {

using VertexDescriptor = std::tuple<size_t, size_t, size_t>;

Material createMaterial(
	const Parser::Materials::value_type& materialDataEntry,
	const milk::graphics::Sampler::Configuration& samplerConfiguration,
	const milk::FilesystemContext& filesystemContext
	)
{
	const auto& materialData = materialDataEntry.second;

	auto material = Material();

	if (materialData.diffuseColour.a() < 0.999f) {
		material.shader() = Material::TRANSPARENT_SHADER;
	} else {
		material.shader() = Material::OPAQUE_SHADER;
	}

	material.set(Material::AMBIENT_COLOUR_PROPERTY, materialData.ambientColour);
	material.set(Material::DIFFUSE_COLOUR_PROPERTY, materialData.diffuseColour);
	material.set(Material::SPECULAR_COLOUR_PROPERTY, materialData.specularColour);
	material.set(Material::SPECULAR_EXPONENT_PROPERTY, materialData.specularExponent);

	material.addTexture(
		Material::DIFFUSE_MAP_TEXTURE,
		filesystemContext.currentWorkingDirectory() / materialData.diffuseMap,
		samplerConfiguration
		);
}

struct VertexDescriptorHasher { // TODO: add tuple hash template to hash_combine?

	size_t operator()(const VertexDescriptor& vertexDescriptor) const {
		size_t seed = 0;
		seed = coconut_tools::utils::hashCombine(seed, std::get<0>(vertexDescriptor));
		seed = coconut_tools::utils::hashCombine(seed, std::get<1>(vertexDescriptor));
		seed = coconut_tools::utils::hashCombine(seed, std::get<2>(vertexDescriptor));
		return seed;
	}

};

} // anonymous namespace

Mesh obj::Importer::import(
	const milk::fs::RawData& data,
	const milk::FilesystemContext& filesystemContext
	)
{
	auto parser = Parser();
	parser.parse(data, filesystemContext);
	
	auto materials = Mesh::Materials();

	auto samplerConfiguration = milk::graphics::Sampler::Configuration();
	samplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR; // TODO

	std::transform(parser.materials().begin(), parser.materials().end(), std::back_inserter(materials),
		[&samplerConfiguration, &filesystemContext](const auto& materialData) {
			return createMaterial(materialData, samplerConfiguration, filesystemContext);
		});

	auto hasFaces = false;
	auto normalsNeedGeneration = false;

	const auto& positionData = parser.positions();
	const auto& textureCoordinateData = parser.textureCoordinateIndex();
	const auto& normalData = parser.normals();
	auto submeshes = Mesh::Submeshes();

	for (const auto& object : parser.objects()) {
		for (auto group : object.groups) {
			if (!group.faces.empty()) {
				hasFaces = true;

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

	if (normalsNeedGeneration) {
		mesh.generateNormals();
	}

	return Mesh(std::move(submeshes), std::move(materials));
}
