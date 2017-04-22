#ifndef _COCONUT_MILK_GRAPHICS_BUFFER_HPP_
#define _COCONUT_MILK_GRAPHICS_BUFFER_HPP_

#include <functional>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"
#include "coconut/milk/utils/IntOfSize.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "Resource.hpp"
#include "ShaderType.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

class Buffer : public Resource {
public:

	enum class CreationPurpose {
		VERTEX_BUFFER = D3D11_BIND_VERTEX_BUFFER,
		INDEX_BUFFER = D3D11_BIND_INDEX_BUFFER,
		CONSTANT_BUFFER = D3D11_BIND_CONSTANT_BUFFER,
	};

	struct Configuration {

		size_t size = 0;

		size_t stride = 0;

		bool allowModifications = false;

		bool allowCPURead = false;

		bool allowGPUWrite = false;

		Configuration() = default;

		Configuration(
			size_t size,
			size_t stride,
			bool allowModifications,
			bool allowCPURead,
			bool allowGPUWrite
			) :
			size(size),
			stride(stride),
			allowModifications(allowModifications),
			allowCPURead(allowCPURead),
			allowGPUWrite(allowGPUWrite)
		{
		}

	};

	Buffer() = default;

	Buffer(Renderer& renderer, CreationPurpose purpose, Configuration configuration, const void* initialData = 0);

	const Configuration& configuration() const noexcept {
		return configuration_;
	}

	ID3D11Buffer* internalBuffer() {
		return buffer_;
	}

	ID3D11Resource& internalResource() override {
		return *buffer_;
	}

private:

	Configuration configuration_;

	system::COMWrapper<ID3D11Buffer> buffer_;

};

CCN_MAKE_POINTER_DEFINITIONS(Buffer);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_BUFFER_HPP_ */
