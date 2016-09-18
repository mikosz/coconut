#ifndef _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_
#define _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_

#include <vector>
#include <memory>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut-tools/enum.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "compile-shader.hpp"
#include "InputLayoutDescription.hpp"

namespace coconut {
namespace milk {
namespace graphics {

#define FLEXIBLE_INPUT_LAYOUT_ELEMENT(name) \
	class name##Element : public FlexibleInputLayoutDescription::Element { \
	public: \
		\
		name##Element(size_t index, FlexibleInputLayoutDescription::Format format); \
		\
		void toElementDesc(D3D11_INPUT_ELEMENT_DESC* desc) const override; \
		\
		size_t size() const override; \
		\
		void make(const VertexInterface& vertex, void* buffer) const override; \
		\
		size_t index() const override { \
			return index_; \
		} \
		\
		const std::string& hlslSemantic() const override { \
			return HLSL_SEMANTIC_; \
		} \
		\
		FlexibleInputLayoutDescription::Format format() const override { \
			return format_; \
		} \
		\
	private: \
		\
		static const std::string HLSL_SEMANTIC_; \
		\
		size_t index_; \
		\
		FlexibleInputLayoutDescription::Format format_; \
		\
	};

class FlexibleInputLayoutDescription : public InputLayoutDescription {
public:

	CCN_MEMBER_ENUM(
		ElementType,
		(POSITION)
		(TEXTURE_COORDINATES)
		(NORMAL)
		);

	CCN_MEMBER_ENUM_VALUES(
		Format,
		(R32G32B32A32_FLOAT)(DXGI_FORMAT_R32G32B32A32_FLOAT)
		(R32G32B32_FLOAT)(DXGI_FORMAT_R32G32B32_FLOAT)
		(R32G32_FLOAT)(DXGI_FORMAT_R32G32_FLOAT)
		);

	class Element {
	public:

		virtual ~Element() {
		}

		virtual void toElementDesc(D3D11_INPUT_ELEMENT_DESC* desc) const = 0;

		virtual size_t size() const = 0;

		virtual void make(const VertexInterface& vertex, void* buffer) const = 0;

		virtual size_t index() const = 0;

		virtual const std::string& hlslSemantic() const = 0;

		virtual Format format() const = 0;

	};

	CCN_MAKE_POINTER_DEFINITIONS(Element);

	FLEXIBLE_INPUT_LAYOUT_ELEMENT(Position);

	FLEXIBLE_INPUT_LAYOUT_ELEMENT(TextureCoordinates);

	FLEXIBLE_INPUT_LAYOUT_ELEMENT(Normal);

	system::COMWrapper<ID3D11InputLayout> makeLayout(
		Renderer& renderer,
		void* shaderData,
		size_t shaderSize
		) const override;

	size_t vertexSize() const override;

	void makeVertex(const VertexInterface& vertex, void* buffer) const override;

	void push(std::shared_ptr<Element> element);

	std::vector<std::uint8_t> createDummyVertexShader() const;

private:

	std::vector<ElementSharedPtr> elements_;

};

CCN_MAKE_POINTER_DEFINITIONS(FlexibleInputLayoutDescription);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_ */
