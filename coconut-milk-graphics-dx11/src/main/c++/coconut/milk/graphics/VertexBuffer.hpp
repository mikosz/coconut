#ifndef _COCONUT_MILK_GRAPHICS_VERTEXBUFFER_HPP_
#define _COCONUT_MILK_GRAPHICS_VERTEXBUFFER_HPP_

#include "Buffer.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class VertexBuffer : public Buffer {
public:

	VertexBuffer() = default;

	VertexBuffer(Renderer& renderer, const Configuration& configuration, const void* initialData = 0) :
		Buffer(renderer, CreationPurpose::VERTEX_BUFFER, configuration, initialData),
		stride_(configuration.stride)
	{
	}

	size_t stride() const {
		return stride_;
	}

private:

	size_t stride_ = 0;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_VERTEXBUFFER_HPP_ */
