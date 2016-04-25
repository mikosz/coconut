#ifndef _COCONUT_MILK_GRAPHICS_DX11_RENDERER_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_RENDERER_HPP_

#include <functional>
#include <memory>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"
#include "coconut/milk/system/Window.hpp"

#include "Texture2d.hpp"
#include "CommandList.hpp"
#include "PrimitiveTopology.hpp"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class Renderer {
public:

	using LockedData = std::unique_ptr<void, std::function<void(void*)>>;

	enum class LockPurpose {
		READ = D3D11_MAP_READ,
		WRITE = D3D11_MAP_WRITE,
		READ_WRITE = D3D11_MAP_READ_WRITE,
		WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
		WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
	};

	struct Configuration {

		bool debugDevice;

		bool vsync;

		std::uint32_t sampleCount; // TODO: verify that this is a power of 2

		std::uint32_t sampleQuality;

	};

	Renderer(system::Window& window, const Configuration& configuration);

	CommandList& getImmediateCommandList();

	CommandList createDeferredCommandList();

	void beginScene();

	void endScene();

	void submit(CommandList& commandList);

	LockedData lock(Data& data, LockPurpose lockPurpose);

	Texture2d& backBuffer() {
		return backBuffer_;
	}

	Texture2d& depthStencil() {
		return depthStencil_;
	}

	ID3D11Device& internalDevice() {
		return *d3dDevice_;
	}

private:

	Configuration configuration_;

	system::COMWrapper<IDXGIAdapter> adapter_;

	system::COMWrapper<ID3D11Device> d3dDevice_;

	CommandList immediateCommandList_;

	system::COMWrapper<IDXGISwapChain> swapChain_;

	Texture2d backBuffer_;

	Texture2d depthStencil_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_RENDERER_HPP_ */
