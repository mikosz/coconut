#ifndef _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_
#define _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_

#include <vector>
#include <memory>

#include <d3d11.h>

#include "InputLayoutDescription.hpp"
#include "coconut/milk/system/COMWrapper.hpp"

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
		void toElementDesc(D3D11_INPUT_ELEMENT_DESC* desc); \
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

	enum Format {
		R32G32B32_FLOAT = DXGI_FORMAT_R32G32B32_FLOAT,
		R32G32_FLOAT = DXGI_FORMAT_R32G32_FLOAT,
	};

	class Element {
	public:

		virtual ~Element() {
		}

		virtual void toElementDesc(D3D11_INPUT_ELEMENT_DESC* desc) = 0;

	};

	FLEXIBLE_INPUT_LAYOUT_ELEMENT(Position);

	FLEXIBLE_INPUT_LAYOUT_ELEMENT(TextureCoordinates);

	system::COMWrapper<ID3D11InputLayout> makeLayout(
		Device& device,
		void* shaderData,
		size_t shaderSize
		);

	void push(std::shared_ptr<Element> element);

private:

	std::vector<std::shared_ptr<Element> > elements_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_FLEXIBLEINPUTLAYOUT_HPP_ */
