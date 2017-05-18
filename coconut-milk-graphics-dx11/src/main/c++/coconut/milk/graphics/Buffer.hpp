#ifndef _COCONUT_MILK_GRAPHICS_BUFFER_HPP_
#define _COCONUT_MILK_GRAPHICS_BUFFER_HPP_

#include <functional>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enums/enum.hpp>

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

	struct Configuration {

		size_t size = 0;

		size_t stride = 0;

		bool allowModifications = false;

		bool allowCPURead = false;

		bool allowGPUWrite = false;

		const void* initialData = nullptr;

		Configuration() = default;

		Configuration(
			size_t size,
			size_t stride,
			bool allowModifications,
			bool allowCPURead,
			bool allowGPUWrite,
			const void* initialData = nullptr
			) :
			size(size),
			stride(stride),
			allowModifications(allowModifications),
			allowCPURead(allowCPURead),
			allowGPUWrite(allowGPUWrite),
			initialData(initialData)
		{
		}

	};

	size_t size() const noexcept;

	ID3D11Buffer* internalResource() const noexcept {
		return reinterpret_cast<ID3D11Buffer*>(Resource::internalResource());
	}

protected:

	CT_MEMBER_ENUM_VALUES(
		CreationPurpose,
		(VERTEX_BUFFER)(D3D11_BIND_VERTEX_BUFFER)
		(INDEX_BUFFER)(D3D11_BIND_INDEX_BUFFER)
		(CONSTANT_BUFFER)(D3D11_BIND_CONSTANT_BUFFER)
		(SHADER_RESOURCE)(D3D11_BIND_SHADER_RESOURCE)
		);

	Buffer() = default;

	Buffer(Renderer& renderer, const Configuration& configuration, CreationPurpose purpose);

private:

	system::COMWrapper<ID3D11Resource> createResource(
		Renderer& renderer,
		const Configuration& configuration,
		CreationPurpose purpose
		);

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_BUFFER_HPP_ */
