#include "Data.hpp"

#include <unordered_map>

#include "coconut/milk/system/cleanup-windows-macros.hpp"
#include "coconut-tools/logger.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::model;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.MODEL.DATA");

void serialise(file_io::Serialiser& serialiser, const milk::math::Vector2d& data) { // TODO: move to vector, move file-io to milk
	serialiser << file_io::Serialiser::labelled("x", data.x());
	serialiser << file_io::Serialiser::labelled("y", data.y());
}

void serialise(file_io::Serialiser& serialiser, const milk::math::Vector3d& data) { // TODO: move to vector, move file-io to milk
	serialiser << file_io::Serialiser::labelled("x", data.x());
	serialiser << file_io::Serialiser::labelled("y", data.y());
	serialiser << file_io::Serialiser::labelled("z", data.z());
}

void serialise(file_io::Serialiser& serialiser, const milk::math::Vector4d& data) { // TODO: move to vector, move file-io to milk
	serialiser << file_io::Serialiser::labelled("x", data.x());
	serialiser << file_io::Serialiser::labelled("y", data.y());
	serialiser << file_io::Serialiser::labelled("z", data.z());
	serialiser << file_io::Serialiser::labelled("w", data.w());
}

void serialise(file_io::Serialiser& serialiser, const Data::DrawGroup& drawGroup) {
	switch (drawGroup.primitiveTopology) {
	case milk::graphics::PrimitiveTopology::TRIANGLE_LIST: // better use a name array
		serialiser << file_io::Serialiser::labelled("primitiveTopology", "TRIANGLE_LIST");
		break;
	case milk::graphics::PrimitiveTopology::TRIANGLE_STRIP:
		serialiser << file_io::Serialiser::labelled("primitiveTopology", "TRIANGLE_STRIP");
		break;
	default:
		assert(0);
	}
	serialiser << file_io::Serialiser::labelled("vertices", drawGroup.vertices);
	serialiser << file_io::Serialiser::labelled("indices", drawGroup.indices);
	serialiser << file_io::Serialiser::labelled("material", drawGroup.material);
}

void serialise(file_io::Serialiser& serialiser, const Data::Material& material) {
	serialiser << file_io::Serialiser::labelled("name", material.name);
	serialiser << file_io::Serialiser::labelled("ambientColour", material.ambientColour);
	serialiser << file_io::Serialiser::labelled("diffuseColour", material.diffuseColour);
	serialiser << file_io::Serialiser::labelled("specularColour", material.specularColour);
	serialiser << file_io::Serialiser::labelled("specularExponent", material.specularExponent);
	serialiser << file_io::Serialiser::labelled("diffuseMap", material.diffuseMap);
}

void serialise(file_io::Serialiser& serialiser, const Data::VertexDescriptor& vertexDescriptor) {
	serialiser << file_io::Serialiser::labelled("positionIndex", vertexDescriptor.positionIndex);
	serialiser << file_io::Serialiser::labelled("normalIndex", vertexDescriptor.normalIndex);
	serialiser << file_io::Serialiser::labelled("textureCoordinateIndex", vertexDescriptor.textureCoordinateIndex);
}

struct Vector3dLexicographicalCompare {

	bool operator()(const milk::math::Vector3d& lhs, const milk::math::Vector3d& rhs) {
		if (lhs == rhs) {
			return false;
		}

		const auto EPSILON = milk::math::Vector3d::EPSILON;

		if (lhs.x() < rhs.x() - EPSILON) {
			return true;
		} else if (lhs.x() - rhs.x() < EPSILON) {
			if (lhs.y() < rhs.y() - EPSILON) {
				return true;
			} else if (lhs.y() - rhs.y() < EPSILON) {
				return lhs.z() < rhs.z();
			}
		}

		return false;
	}

};

} // anonymous namespace

void Data::generateNormals() {
	CT_LOG_DEBUG << "Generating normals";

	for (size_t groupIndex = 0; groupIndex < drawGroups.size(); ++groupIndex) {
		auto& currentGroup = drawGroups[groupIndex];

		assert(currentGroup.primitiveTopology == milk::graphics::PrimitiveTopology::TRIANGLE_LIST); // TODO refacor, extend supported topologies

		std::unordered_multimap<size_t, size_t> affectingFaces;

		for (size_t faceIdx = 0; faceIdx < currentGroup.indices.size(); faceIdx += 3) {
			assert(faceIdx + 2 < currentGroup.indices.size());
			for (size_t faceVertexIdx = faceIdx; faceVertexIdx < faceIdx + 3; ++faceVertexIdx) {
				const auto vertexIdx = currentGroup.indices[faceVertexIdx];
				affectingFaces.emplace(std::make_pair(vertexIdx, faceIdx));
			}
		}

		std::unordered_map<size_t, milk::math::Vector3d> faceNormals;
		std::map<milk::math::Vector3d, size_t, Vector3dLexicographicalCompare> storedNormals;

		for (size_t vertexIdx = 0; vertexIdx < currentGroup.vertices.size(); ++vertexIdx) {
			auto range = affectingFaces.equal_range(vertexIdx);

			milk::math::Vector3d normal(0.0f, 0.0f, 0.0f);

			CT_LOG_TRACE << "Calculating normal for vertex " << vertexIdx << ": " << positions[currentGroup.vertices[vertexIdx].positionIndex];

			while (range.first != range.second) {
				const auto faceIdx = range.first->second;
				if (faceNormals.count(faceIdx) == 0) {
					// TODO: kolejnosc
					milk::math::Vector3d a =
						positions[currentGroup.vertices[currentGroup.indices[faceIdx + 1]].positionIndex] -
						positions[currentGroup.vertices[currentGroup.indices[faceIdx]].positionIndex];
					milk::math::Vector3d b =
						positions[currentGroup.vertices[currentGroup.indices[faceIdx + 2]].positionIndex] -
						positions[currentGroup.vertices[currentGroup.indices[faceIdx]].positionIndex];

					CT_LOG_TRACE << "Face vectors: " << a << " x " << b << " = " << a.cross(b);

					faceNormals.insert(std::make_pair(
						faceIdx,
						a.cross(b)
						));
				}

				normal += faceNormals[faceIdx];

				CT_LOG_TRACE << "Temporary normal with face " << (faceIdx / 3) << " is " << normal;

				++range.first;
			}

			normal.normalise();

			CT_LOG_TRACE << "Final normal is " << normal;

			auto found = storedNormals.find(normal);
			if (found != storedNormals.end()) {
				currentGroup.vertices[vertexIdx].normalIndex = found->second;
			} else {
				normals.emplace_back(normal);
				const auto normalIndex = normals.size() - 1;
				currentGroup.vertices[vertexIdx].normalIndex = normalIndex;
				storedNormals.insert(std::make_pair(normal, normalIndex));
			}
		}
	}
}

void coconut::pulp::model::serialise(file_io::Serialiser& serialiser, const Data& data) {
	serialiser << file_io::Serialiser::labelled("positions", data.positions);
	serialiser << file_io::Serialiser::labelled("normals", data.normals);
	serialiser << file_io::Serialiser::labelled("textureCoordinates", data.textureCoordinates);
	serialiser << file_io::Serialiser::labelled("drawGroups", data.drawGroups);
}
