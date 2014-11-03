#ifndef _COCONUT_MILK_GRAPHICS_BUFFER_HPP_
#define _COCONUT_MILK_GRAPHICS_BUFFER_HPP_

#include <d3d11.h>

#include "system/COMWrapper.hpp"
#include "utils/IntOfSize.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class Buffer {
public:

	enum CreationPurpose {
		VERTEX_BUFFER = D3D11_BIND_VERTEX_BUFFER,
		INDEX_BUFFER = D3D11_BIND_INDEX_BUFFER,
		CONSTANT_BUFFER = D3D11_BIND_CONSTANT_BUFFER,
	};

	enum LockPurpose {
		READ = D3D11_MAP_READ,
		WRITE = D3D11_MAP_WRITE,
		READ_WRITE = D3D11_MAP_READ_WRITE,
		WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
		WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
	};

	enum ShaderType {
		VERTEX_SHADER,
		PIXEL_SHADER
	};

	struct Configuration {

		size_t size;

		size_t stride;

		bool allowModifications;

		bool allowCPURead;

		bool allowGPUWrite;

		CreationPurpose purpose;

	};

	Buffer(Device& device, const Configuration& configuration, void* initialData = 0);

	void* lock(Device& device, LockPurpose lockPurpose);

	void unlock(Device& device);

	void bind(Device& device, ShaderType shaderType, size_t slot);

	ID3D11Buffer* resource() {
		return buffer_.get();
	}

private:

	Configuration configuration_;

	system::COMWrapper<ID3D11Buffer> buffer_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_BUFFER_HPP_ */
