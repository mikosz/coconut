#ifndef _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_
#define _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_

#include <vector>
#include <memory>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enum.hpp>

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "compile-shader.hpp"
#include "InputLayoutDescription.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class FlexibleInputLayoutDescription : public InputLayoutDescription {
public:

	CCN_MEMBER_ENUM(
		ElementType,
		(POSITION)
		(TEXTURE_COORDINATES)
		(NORMAL)
		(INSTANCE_ID)
		);

	CCN_MEMBER_ENUM_VALUES(
		Format,
		(R32G32B32A32_FLOAT)(DXGI_FORMAT_R32G32B32A32_FLOAT)
		(R32G32B32_FLOAT)(DXGI_FORMAT_R32G32B32_FLOAT)
		(R32G32_FLOAT)(DXGI_FORMAT_R32G32_FLOAT)
		(R32_UINT)(DXGI_FORMAT_R32_UINT)
		);

	struct Element {
	public:

		Element(
			ElementType type,
			size_t semanticIndex,
			FlexibleInputLayoutDescription::Format format,
			SlotType inputSlotType,
			size_t instanceDataStepRate = 0
			) :
			type(type),
			semanticIndex(semanticIndex),
			format(format),
			inputSlotType(inputSlotType),
			instanceDataStepRate(instanceDataStepRate)
		{
		}

		ElementType type;

		size_t semanticIndex;

		FlexibleInputLayoutDescription::Format format;

		SlotType inputSlotType;

		size_t instanceDataStepRate;

		void make(const VertexInterface& vertex, void* buffer) const;

	};

	static const std::string POSITION_SEMANTIC;
	static const std::string TEXTURE_ELEMENT_SEMANTIC;
	static const std::string NORMAL_SEMANTIC;
	static const std::string INSTANCE_ID_SEMANTIC;

	system::COMWrapper<ID3D11InputLayout> makeLayout(
		Renderer& renderer,
		const void* shaderData,
		size_t shaderSize
		) const override;

	size_t vertexSize(SlotType slotType) const override;

	void makeVertex(const VertexInterface& vertex, void* buffer, SlotType slotType) const override;

	void push(Element element);

	std::vector<std::uint8_t> createDummyVertexShader() const;

private:

	std::vector<Element> elements_;

};

CCN_MAKE_POINTER_DEFINITIONS(FlexibleInputLayoutDescription);

#undef FLEXIBLE_INPUT_LAYOUT_ELEMENT

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_ */
