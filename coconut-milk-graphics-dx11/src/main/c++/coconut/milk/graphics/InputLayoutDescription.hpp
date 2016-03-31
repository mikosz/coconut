#ifndef _COCONUT_MILK_GRAPHICS_INPUTLAYOUTDESCRIPTION_HPP_
#define _COCONUT_MILK_GRAPHICS_INPUTLAYOUTDESCRIPTION_HPP_

#include <d3d11.h>

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "VertexInterface.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

class InputLayoutDescription {
public:

	virtual ~InputLayoutDescription() {
	}

	InputLayoutDescription(const InputLayoutDescription&) = delete;

	void operator=(const InputLayoutDescription&) = delete;

	virtual void makeVertex(const VertexInterface& vertex, void* buffer) const = 0;

	virtual system::COMWrapper<ID3D11InputLayout> makeLayout(
		Renderer& renderer,
		void* shaderData,
		size_t shaderSize
		) const = 0;

	virtual size_t vertexSize() const = 0;

protected:

	InputLayoutDescription() {
	}

};

MAKE_POINTER_DEFINITIONS(InputLayoutDescription);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_INPUTLAYOUTDESCRIPTION_HPP_ */
