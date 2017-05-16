#ifndef _COCONUT_MILK_GRAPHICS_CONSTANTBUFFER_HPP_
#define _COCONUT_MILK_GRAPHICS_CONSTANTBUFFER_HPP_

#include "Buffer.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class ConstantBuffer : public Buffer { // TODO: this, Vertex and IndexBuffer classes are probably useless.
public:

	ConstantBuffer(Renderer& renderer, const Configuration& configuration) :
		Buffer(renderer, configuration, CreationPurpose::CONSTANT_BUFFER)
	{
	}

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_CONSTANTBUFFER_HPP_ */
