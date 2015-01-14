#include "Model.hpp"

#include <cstdint>

#include <DirectXMath.h>

#include "shader/ShaderFactory.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

struct V {
	DirectX::XMFLOAT3 pos;
};

milk::graphics::Buffer::Configuration conf() {
	milk::graphics::Buffer::Configuration c;
	c.allowCPURead = false;
	c.allowGPUWrite = false;
	c.allowModifications = true;
	c.size = 3 * sizeof(V);
	c.stride = sizeof(V);
	c.purpose = milk::graphics::Buffer::CreationPurpose::VERTEX_BUFFER;

	return c;
}

milk::graphics::Buffer::Configuration iconf() {
	milk::graphics::Buffer::Configuration c;
	c.allowCPURead = false;
	c.allowGPUWrite = false;
	c.allowModifications = true;
	c.size = 3 * sizeof(std::uint16_t);
	c.stride = sizeof(std::uint16_t);
	c.purpose = milk::graphics::Buffer::CreationPurpose::INDEX_BUFFER;

	return c;
}

Model::Model(milk::graphics::Device& device, model_loader::ModelLoader& loader) :
	vertexBuffer_(device, conf(), 0),
	indexBuffer_(device, iconf(), 0)
{
	/* {
		milk::graphics::Buffer::LockedData data = vertexBuffer_.lock(device, milk::graphics::Buffer::LockPurpose::WRITE_DISCARD);
		V* vs = (V*)data.data;

		vs[0].pos.x = -0.5f;
		vs[0].pos.y = -0.5f;
		vs[0].pos.z = -0.5f;
		
		vs[1].pos.x = 0.0f;
		vs[1].pos.y = 0.5f;
		vs[1].pos.z = -0.5f;

		vs[2].pos.x = 0.5f;
		vs[2].pos.y = -0.5f;
		vs[2].pos.z = -0.5f;
	}

	{
		milk::graphics::Buffer::LockedData data = indexBuffer_.lock(device, milk::graphics::Buffer::LockPurpose::WRITE_DISCARD);
		std::uint16_t* is = (std::uint16_t*)data.data;

		is[0] = 0;
		is[1] = 1;
		is[2] = 2;
	}

	MaterialSharedPtr material(new Material(device));
	SmoothingGroup group;
	group.firstIndex = 0;
	group.indexCount = 3;

	smoothingGroupsByMaterial_.insert(std::make_pair(material, group)); */

	DataListener listener(*this);
	loader.load(listener);
}

void Model::render(milk::graphics::Device& device, RenderingContext renderingContext) {
	renderingContext.model = this;

	vertexBuffer_.bind(device, milk::graphics::Buffer::ShaderType::VERTEX, 0);
	indexBuffer_.bind(device, milk::graphics::Buffer::ShaderType::VERTEX, 0);

	for (auto smoothingGroup : smoothingGroupsByMaterial_) {
		smoothingGroup.first->bind(device, renderingContext);

		device.d3dDeviceContext()->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(smoothingGroup.second.primitiveTopology));
		device.d3dDeviceContext()->DrawIndexed(smoothingGroup.second.indexCount, smoothingGroup.second.firstIndex, 0);
	}
}

Model::DataListener::DataListener(Model& model) :
	model_(model)
{
}

void Model::DataListener::newObject() {
}

void Model::DataListener::newSmoothingGroup(milk::graphics::PrimitiveTopology primitiveTopology) {
}

void Model::DataListener::newFace() {
}

void Model::DataListener::newVertex() {
}

void Model::DataListener::setVertexPosition(const milk::math::Vector3d& position) {
}

void Model::DataListener::setVertexTextureCoordinate(const milk::math::Vector2d& textureCoordinate) {
}

void Model::DataListener::setVertexNormal(const milk::math::Vector3d& normal) {
}

void Model::DataListener::setVertexNormalNeedsCalculation() {
}

void Model::DataListener::endModel() {
}