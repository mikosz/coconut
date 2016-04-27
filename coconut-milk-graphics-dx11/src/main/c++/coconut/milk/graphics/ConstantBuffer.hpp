#ifndef _COCONUT_MILK_GRAPHICS_CONSTANTBUFFER_HPP_
#define _COCONUT_MILK_GRAPHICS_CONSTANTBUFFER_HPP_

#include "Buffer.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class ConstantBuffer : public Buffer {
public:

	ConstantBuffer(Renderer& renderer, const Configuration& configuration, const void* initialData = 0) :
		Buffer(renderer, CreationPurpose::CONSTANT_BUFFER, configuration, initialData)
	{
	}

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_CONSTANTBUFFER_HPP_ */
