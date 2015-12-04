#include "ObjModelLoader.hpp"

#include "coconut/milk/graphics/PrimitiveTopology.hpp"
#include "coconut/milk/graphics/ImageLoader.hpp"
#include "coconut/milk/graphics/Texture2d.hpp"

#include "../material/PhongMaterial.hpp"
#include "../material/MaterialLibrary.hpp"
#include "ObjModelParser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::model_loader;

void ObjModelLoader::load(ModelDataListener& modelDataListener, milk::graphics::Device& graphicsDevice) {
	ObjModelParser parser;
	parser.parse(*is_, *materialFileOpener_);

	bool hasFaces = false;

	const auto& materials = parser.materials();
	const auto& positions = parser.positions();
	const auto& textureCoordinates = parser.textureCoordinates();
	const auto& normals = parser.normals();

	{ // TODO: extract function
		material::MaterialLibrary materialLibrary;

		milk::graphics::ImageLoader imageLoader;

		for (const auto& materialData : materials) {
			auto material = static_cast<material::MaterialSharedPtr>(std::make_shared<material::PhongMaterial>());

			material->setDiffuseColour(milk::math::Vector4d(materialData.second.diffuseColour, 1.0f));
			material->setAmbientColour(milk::math::Vector4d(materialData.second.ambientColour, 1.0f));
			material->setSpecularColour(milk::math::Vector4d(materialData.second.specularColour, 1.0f));
			material->setSpecularExponent(materialData.second.specularExponent);

			if (!materialData.second.diffuseMap.empty()) {
				auto image = imageLoader.load(materialFileOpener_->pathTo(materialData.second.diffuseMap));
				material->setDiffuseMap(std::make_unique<milk::graphics::Texture2d>(graphicsDevice, image));
			}

			materialLibrary.put(materialData.first, material);
		}

		modelDataListener.setMaterialLibrary(std::move(materialLibrary));
	}

	for (size_t objectIndex = 0; objectIndex < parser.objects().size(); ++objectIndex) {
		const ObjModelParser::Object& object = parser.objects()[objectIndex];

		for (auto group : object.groups) {
			hasFaces = hasFaces || !group.faces.empty();

			if (hasFaces) {
				bool normalsNeedGeneration = false;

				modelDataListener.setMaterialName(group.material);

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
