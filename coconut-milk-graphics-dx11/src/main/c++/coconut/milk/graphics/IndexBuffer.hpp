#ifndef _COCONUT_MILK_GRAPHICS_INDEXBUFFER_HPP_
#define _COCONUT_MILK_GRAPHICS_INDEXBUFFER_HPP_

#include <cassert>
#include <cstdint>

#include "Buffer.hpp"
#include "PixelFormat.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class IndexBuffer : public Buffer {
public:

	template <class IntegralType>
	static IntegralType degenerateIndex();

	IndexBuffer(Renderer& renderer, const Configuration& configuration, const void* initialData = 0) :
		Buffer(renderer, CreationPurpose::INDEX_BUFFER, configuration, initialData),
		pixelFormat_(configuration.stride == 2 ? PixelFormat::I16_UINT : PixelFormat::I32_UINT)
	{
		assert(configuration.stride == 2 || configuration.stride == 4);
	}

	PixelFormat pixelFormat() const {
		return pixelFormat_;
	}

private:

	PixelFormat pixelFormat_;

};

template <>
inline std::uint16_t IndexBuffer::degenerateIndex<std::uint16_t>() {
	return static_cast<std::uint16_t>(-1);
}

template <>
inline std::uint32_t IndexBuffer::degenerateIndex<std::uint32_t>() {
	return static_cast<std::uint32_t>(-1);
}

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_INDEXBUFFER_HPP_ */
