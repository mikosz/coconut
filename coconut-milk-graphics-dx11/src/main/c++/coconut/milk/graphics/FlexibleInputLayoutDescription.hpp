#ifndef _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_
#define _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_

#include <vector>
#include <memory>

#include <d3d11.h>

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "InputLayoutDescription.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

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
	private: \
		\
		size_t index_; \
		\
		FlexibleInputLayoutDescription::Format format_; \
		\
	};

class FlexibleInputLayoutDescription : public InputLayoutDescription {
public:

	enum class Format {
		R32G32B32A32_FLOAT = DXGI_FORMAT_R32G32B32A32_FLOAT,
		R32G32B32_FLOAT = DXGI_FORMAT_R32G32B32_FLOAT,
		R32G32_FLOAT = DXGI_FORMAT_R32G32_FLOAT,
	};

	class Element {
	public:

		virtual ~Element() {
		}

		virtual void toElementDesc(D3D11_INPUT_ELEMENT_DESC* desc) const = 0;

		virtual size_t size() const = 0;

		virtual void make(const VertexInterface& vertex, void* buffer) const = 0;

	};

	MAKE_POINTER_DEFINITIONS(Element);

	FLEXIBLE_INPUT_LAYOUT_ELEMENT(Position);

	FLEXIBLE_INPUT_LAYOUT_ELEMENT(TextureCoordinates);

	system::COMWrapper<ID3D11InputLayout> makeLayout(
		Device& device,
		void* shaderData,
		size_t shaderSize
		) const override;

	size_t vertexSize() const override;

	void makeVertex(const VertexInterface& vertex, void* buffer) const override;

	void push(std::shared_ptr<Element> element);

private:

	std::vector<ElementSharedPtr> elements_;

};

MAKE_POINTER_DEFINITIONS(FlexibleInputLayoutDescription);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_ */
