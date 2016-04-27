#ifndef _COCONUT_MILK_GRAPHICS_PRIMITIVETOPOLOGY_HPP_
#define _COCONUT_MILK_GRAPHICS_PRIMITIVETOPOLOGY_HPP_

#include <string>

#include <D3DCommon.h>

#include <coconut-tools/enum.hpp>

namespace coconut {
namespace milk {
namespace graphics {

CCN_ENUM_VALUES(
	PrimitiveTopology,
	(INVALID)(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED)
	(TRIANGLE_LIST)(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	(TRIANGLE_STRIP)(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
	);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_PRIMITIVETOPOLOGY_HPP_ */
