#ifndef _COCONUT_MILK_GRAPHICS_PRIMITIVETOPOLOGY_HPP_
#define _COCONUT_MILK_GRAPHICS_PRIMITIVETOPOLOGY_HPP_

#include <D3DCommon.h>

namespace coconut {
namespace milk {
namespace graphics {

enum class PrimitiveTopology {
	TRIANGLE_LIST = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	TRIANGLE_STRIP = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_PRIMITIVETOPOLOGY_HPP_ */
