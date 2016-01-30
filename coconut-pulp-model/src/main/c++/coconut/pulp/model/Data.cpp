#include "Data.hpp"

#include <unordered_map>

#include "coconut/milk/system/cleanup-windows-macros.hpp"
#include "coconut-tools/logger.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::model;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.MODEL.DATA");

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

			normals.emplace_back(normal);
			currentGroup.vertices[vertexIdx].normalIndex = normals.size() - 1;
		}
	}
}
