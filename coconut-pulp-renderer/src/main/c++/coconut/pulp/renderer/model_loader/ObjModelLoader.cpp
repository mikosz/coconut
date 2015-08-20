#include "ObjModelLoader.hpp"

#include "coconut/milk/graphics/PrimitiveTopology.hpp"

#include "ObjModelParser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::model_loader;

void ObjModelLoader::load(ModelDataListener& modelDataListener) {
	ObjModelParser parser;
	parser.parse(*is_, *materialFileOpener_);

	bool hasFaces = false;

	const ObjModelParser::Positions& positions = parser.positions();
	const ObjModelParser::TextureCoordinates& textureCoordinates = parser.textureCoordinates();
	const ObjModelParser::Normals& normals = parser.normals();

	for (size_t objectIndex = 0; objectIndex < parser.objects().size(); ++objectIndex) {
		const ObjModelParser::Object& object = parser.objects()[objectIndex];

		for (auto group : object.groups) {
			hasFaces = hasFaces || !group.faces.empty();

			if (hasFaces) {
				bool normalsNeedGeneration = false;

				ObjModelParser::Materials::const_iterator materialIt = parser.materials().find(group.material);
				if (materialIt == parser.materials().end()) {
					throw std::runtime_error("Material " + group.material + " not found");
				} else {
					modelDataListener.setMaterialName(materialIt->second.name);
					modelDataListener.setAmbientColour(materialIt->second.ambientColour.widen(1.0f));
					modelDataListener.setDiffuseColour(materialIt->second.diffuseColour.widen(1.0f));
					modelDataListener.setSpecularColour(materialIt->second.specularColour.widen(1.0f));
					modelDataListener.setSpecularExponent(materialIt->second.specularExponent);
				}

				for (size_t faceIndex = 0; faceIndex < group.faces.size(); ++faceIndex) {
					const ObjModelParser::Face& face = group.faces[faceIndex];

					for (auto vertex : face.vertices) {
						modelDataListener.setVertexPosition(positions[vertex.positionIndex]);
						modelDataListener.setVertexTextureCoordinate(textureCoordinates[vertex.textureCoordinateIndex]);
						if (vertex.normalIndex != ObjModelParser::NORMAL_INDEX_UNKNOWN) {
							modelDataListener.setVertexNormal(normals[vertex.normalIndex]);
						} else {
							modelDataListener.setVertexNormalNeedsCalculation();
						}

						modelDataListener.endVertex();
					}

					modelDataListener.endFace();
				}

				modelDataListener.endSmoothingGroup(milk::graphics::PrimitiveTopology::TRIANGLE_LIST);
			}
		}

		modelDataListener.endObject();
	}

	if (!hasFaces) {
		throw std::runtime_error("Model has no faces");
	}

	modelDataListener.endModel();
}
