#include "Importer.hpp"

#include <unordered_map>
#include <tuple>

#include "Parser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::model;
using namespace coconut::pulp::model::obj;

namespace /* anonymous */ {

using VertexDescriptor = std::tuple<size_t, size_t, size_t>;

size_t hashCombine(size_t seed, size_t value) { // TODO: extract to tools
	// Code from boost
	// Reciprocal of the golden ratio helps spread entropy and handles duplicates.
	// See Mike Seymour in magic-numbers-in-boosthash-combine: http://stackoverflow.com/questions/4948780
	
	seed ^= std::hash_value(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

milk::math::Vector4d rgbToRgba(const milk::math::Vector3d& rgbColour) { // TODO: sort out a colour type
	return milk::math::Vector4d(rgbColour, 1.0f);
}

} // anonymous namespace

namespace std {

template <>
struct hash<VertexDescriptor> {

	size_t operator()(const VertexDescriptor& vertexDescriptor) const {
		size_t seed = 0;
		seed = hashCombine(seed, std::get<0>(vertexDescriptor));
		seed = hashCombine(seed, std::get<1>(vertexDescriptor));
		seed = hashCombine(seed, std::get<2>(vertexDescriptor));
		return seed;
	}

};

} // namespace std

Data obj::Importer::import(std::istream& is) {
	Parser parser;
	parser.parse(is, *materialFileOpener_);

	Data modelData;

	bool hasFaces = false;

	const auto& materials = parser.materials();
	modelData.positions = parser.positions();
	modelData.textureCoordinates = parser.textureCoordinateIndex();
	modelData.normals = parser.normals();

	auto normalsNeedGeneration = false;

	// TODO: objects are merged here, is this ok? will we ever have more than one object?
	for (size_t objectIndex = 0; objectIndex < parser.objects().size(); ++objectIndex) {
		const auto& object = parser.objects()[objectIndex];

		for (auto group : object.groups) {
			hasFaces = hasFaces || !group.faces.empty();

			if (hasFaces) {
				modelData.drawGroups.emplace_back();
				auto& currentGroupData = modelData.drawGroups.back();

				currentGroupData.primitiveTopology = milk::graphics::PrimitiveTopology::TRIANGLE_LIST;

				const auto& materialData = parser.materials().at(group.material);
				currentGroupData.material.name = materialData.name;
				currentGroupData.material.ambientColour = rgbToRgba(materialData.ambientColour);
				currentGroupData.material.diffuseColour = rgbToRgba(materialData.diffuseColour);
				currentGroupData.material.diffuseMap = materialFileOpener_->pathTo(materialData.diffuseMap).string();
				currentGroupData.material.specularColour = rgbToRgba(materialData.diffuseColour);
				currentGroupData.material.specularExponent = materialData.specularExponent;

				std::unordered_map<VertexDescriptor, size_t> vertexIndices;

				for (size_t faceIndex = 0; faceIndex < group.faces.size(); ++faceIndex) {
					const auto& face = group.faces[faceIndex];

					for (auto vertex : face.vertices) {
						VertexDescriptor vertexDesc(vertex.positionIndex, vertex.textureCoordinateIndex, vertex.normalIndex);
						if (vertexIndices.count(vertexDesc) == 0) {
							currentGroupData.vertices.emplace_back();
							
							auto& currentVertexData = currentGroupData.vertices.back();
							currentVertexData.positionIndex = vertex.positionIndex;
							currentVertexData.normalIndex = vertex.normalIndex;
							currentVertexData.textureCoordinateIndex = vertex.textureCoordinateIndex;
							
							if (vertex.normalIndex == Parser::NORMAL_INDEX_UNKNOWN) {
								normalsNeedGeneration = true;
							}

							vertexIndices.insert(std::make_pair(vertexDesc, currentGroupData.vertices.size() - 1));
						}

						currentGroupData.indices.emplace_back(vertexIndices[vertexDesc]);
					}
				}
			}
		}
	}

	if (!hasFaces) {
		throw std::runtime_error("Model has no faces");
	}

	if (normalsNeedGeneration) {
		modelData.generateNormals();
	}

	return modelData;
}
