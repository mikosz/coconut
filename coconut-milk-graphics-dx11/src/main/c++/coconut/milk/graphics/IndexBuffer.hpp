#ifndef _COCONUT_MILK_GRAPHICS_INDEXBUFFER_HPP_
#define _COCONUT_MILK_GRAPHICS_INDEXBUFFER_HPP_

#include "Buffer.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class IndexBuffer : public Buffer {
public:

	IndexBuffer(Renderer& renderer, const Configuration& configuration, const void* initialData = 0) :
		Buffer(renderer, CreationPurpose::INDEX_BUFFER, configuration, initialData)
	{
	}

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_INDEXBUFFER_HPP_ */
