#ifndef _COCONUT_MILK_GRAPHICS_INPUTLAYOUTDESCRIPTION_HPP_
#define _COCONUT_MILK_GRAPHICS_INPUTLAYOUTDESCRIPTION_HPP_

#include <vector>
#include <functional>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enum.hpp>

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "PixelFormat.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

class InputLayout final {
public:

	CCN_MEMBER_ENUM_VALUES(
		SlotType,
		(PER_VERTEX_DATA)(D3D11_INPUT_PER_VERTEX_DATA)
		(PER_INSTANCE_DATA)(D3D11_INPUT_PER_INSTANCE_DATA)
		);

	struct Element {
	public:

		Element(
			std::string semantic,
			size_t semanticIndex,
			PixelFormat format,
			SlotType inputSlotType,
			size_t instanceDataStepRate
			);

		std::string semantic;

		size_t semanticIndex;

		PixelFormat format;

		SlotType inputSlotType;

		size_t instanceDataStepRate;

	};

	using Elements = std::vector<Element>;

	InputLayout(
		Renderer& renderer,
		const Elements& elements
		);

	ID3D11InputLayout& internalInputLayout() const {
		return *dxInputLayout_;
	}

private:

	mutable system::COMWrapper<ID3D11InputLayout> dxInputLayout_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_INPUTLAYOUTDESCRIPTION_HPP_ */
