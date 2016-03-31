#ifndef _COCONUT_MILK_GRAPHICS_INPUTLAYOUT_HPP_
#define _COCONUT_MILK_GRAPHICS_INPUTLAYOUT_HPP_

#include "Renderer.hpp"
#include "InputLayoutDescription.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class InputLayout {
public:

	InputLayout(ConstInputLayoutDescriptionUniquePtr description, Renderer& renderer, void* shaderData, size_t shaderSize) :
		description_(std::move(description)),
		dxInputLayout_(description_->makeLayout(renderer, shaderData, shaderSize))
	{
	}

	void bind(Renderer& renderer) const {
		renderer.internalDeviceContext().IASetInputLayout(dxInputLayout_);
	}

	const InputLayoutDescription& description() const {
		return *description_;
	}

private:

	ConstInputLayoutDescriptionUniquePtr description_;

	mutable system::COMWrapper<ID3D11InputLayout> dxInputLayout_; // because PSSetShader takes InputLayout* not const InputLayout*

};

MAKE_POINTER_DEFINITIONS(InputLayout);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_INPUTLAYOUT_HPP_ */

