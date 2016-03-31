#ifndef _COCONUT_MILK_GRAPHICS_BUFFER_HPP_
#define _COCONUT_MILK_GRAPHICS_BUFFER_HPP_

#include <functional>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"
#include "coconut/milk/utils/IntOfSize.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "Data.hpp"
#include "ShaderType.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

class Buffer : public Data {
public:

	enum class CreationPurpose {
		VERTEX_BUFFER = D3D11_BIND_VERTEX_BUFFER,
		INDEX_BUFFER = D3D11_BIND_INDEX_BUFFER,
		CONSTANT_BUFFER = D3D11_BIND_CONSTANT_BUFFER,
	};

	struct Configuration {

		size_t size;

		size_t stride;

		bool allowModifications;

		bool allowCPURead;

		bool allowGPUWrite;

		CreationPurpose purpose;

		Configuration() {
		}

		Configuration(
			size_t size,
			size_t stride,
			bool allowModifications,
			bool allowCPURead,
			bool allowGPUWrite,
			CreationPurpose purpose
			) :
			size(size),
			stride(stride),
			allowModifications(allowModifications),
			allowCPURead(allowCPURead),
			allowGPUWrite(allowGPUWrite),
			purpose(purpose)
		{
		}

	};

	Buffer(Renderer& renderer, const Configuration& configuration, const void* initialData = 0);

	void bind(Renderer& renderer, ShaderType shaderType, size_t slot);

	ID3D11Buffer& internalBuffer() {
		return buffer_;
	}

	ID3D11Resource& internalResource() override {
		return buffer_;
	}

private:

	Configuration configuration_;

	system::COMWrapper<ID3D11Buffer> buffer_;

};

MAKE_POINTER_DEFINITIONS(Buffer);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_BUFFER_HPP_ */
