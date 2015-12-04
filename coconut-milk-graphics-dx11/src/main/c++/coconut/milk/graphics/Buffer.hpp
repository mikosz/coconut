#ifndef _COCONUT_MILK_GRAPHICS_BUFFER_HPP_
#define _COCONUT_MILK_GRAPHICS_BUFFER_HPP_

#include <d3d11.h>

#include "coconut/milk/system/COMWrapper.hpp"
#include "coconut/milk/utils/IntOfSize.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"
#include "coconut/milk/utils/RAIIHelper.hpp"

#include "ShaderType.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class Buffer {
public:

	enum class CreationPurpose {
		VERTEX_BUFFER = D3D11_BIND_VERTEX_BUFFER,
		INDEX_BUFFER = D3D11_BIND_INDEX_BUFFER,
		CONSTANT_BUFFER = D3D11_BIND_CONSTANT_BUFFER,
	};

	enum class LockPurpose {
		READ = D3D11_MAP_READ,
		WRITE = D3D11_MAP_WRITE,
		READ_WRITE = D3D11_MAP_READ_WRITE,
		WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
		WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
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

	struct LockedData {

		void* data;

	private:

		utils::RAIIHelper unlocker_;

		LockedData() :
			data(0)
		{
		}

		friend class Buffer;

	};

	Buffer(Device& device, const Configuration& configuration, const void* initialData = 0);

	LockedData lock(Device& device, LockPurpose lockPurpose);

	void bind(Device& device, ShaderType shaderType, size_t slot);

	ID3D11Buffer* resource() {
		return buffer_.get();
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
