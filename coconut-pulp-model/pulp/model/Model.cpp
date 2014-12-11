#include "Model.hpp"

#include <cstdint>

#include <DirectXMath.h>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::model;

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
	c.purpose = milk::graphics::Buffer::VERTEX_BUFFER;

	return c;
}

milk::graphics::Buffer::Configuration iconf() {
	milk::graphics::Buffer::Configuration c;
	c.allowCPURead = false;
	c.allowGPUWrite = false;
	c.allowModifications = true;
	c.size = 3 * sizeof(std::uint16_t);
	c.stride = sizeof(std::uint16_t);
	c.purpose = milk::graphics::Buffer::INDEX_BUFFER;

	return c;
}

Model::Model(milk::graphics::Device& device, std::shared_ptr<milk::graphics::VertexShader> vertexShader, std::shared_ptr<milk::graphics::PixelShader> pixelShader) :
	vertexBuffer_(device, conf(), 0),
	indexBuffer_(device, iconf(), 0),
	vertexShader_(vertexShader),
	pixelShader_(pixelShader),
	worldMatrix_(std::bind(&Model::calculateWorldTransformation, this, std::placeholders::_1))
{
	{
		V* vs = (V*)vertexBuffer_.lock(device, milk::graphics::Buffer::WRITE_DISCARD);

		vs[0].pos.x = -0.5f;
		vs[0].pos.y = -0.5f;
		vs[0].pos.z = -0.5f;

		vs[1].pos.x = 0.0f;
		vs[1].pos.y = 0.5f;
		vs[1].pos.z = -0.5f;

		vs[2].pos.x = 0.5f;
		vs[2].pos.y = -0.5f;
		vs[2].pos.z = -0.5f;

		vertexBuffer_.unlock(device);
	}

	{
		std::uint16_t* is = (std::uint16_t*)indexBuffer_.lock(device, milk::graphics::Buffer::WRITE_DISCARD);

		is[0] = 0;
		is[1] = 1;
		is[2] = 2;

		indexBuffer_.unlock(device);
	}
}

void Model::render(milk::graphics::Device& device) {
	vertexBuffer_.bind(device, milk::graphics::Buffer::VERTEX_SHADER, 0);
	indexBuffer_.bind(device, milk::graphics::Buffer::VERTEX_SHADER, 0);

	vertexShader_->bind(device, *this);
	pixelShader_->bind(device);

	device.d3dDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device.d3dDeviceContext()->DrawIndexed(3, 0, 0);
}

const milk::math::Matrix& Model::worldTransformation() const {
	return worldMatrix_.get();
}

void Model::calculateWorldTransformation(milk::math::Matrix& matrix) {
	matrix =
		milk::math::Matrix::rotation(rotation_.z(), rotation_.x(), rotation_.y()) *
		milk::math::Matrix::scale(scale_.x(), scale_.y(), scale_.z()) *
		milk::math::Matrix::translation(translation_.x(), translation_.y(), translation_.z())
		;
}
