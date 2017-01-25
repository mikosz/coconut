#ifndef _COCONUT_MILK_GRAPHICS_TESTSUITE_HPP_
#define _COCONUT_MILK_GRAPHICS_TESTSUITE_HPP_

#include <array>

#include "coconut/milk/system/App.hpp"
#include "coconut/milk/system/Window.hpp"

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Rasteriser.hpp"
#include "Viewport.hpp"
#include "Renderer.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class TestSuite {
public:

	TestSuite() :
		app_(std::make_unique<system::App>(*system::GlobalMainArguments::instance())),
		window_(windowConfiguration(), app_),
		renderer_(window_, rendererConfiguration()),
		rasteriser_(renderer_, rasteriserConfiguration()),
		viewport_(viewportConfiguration()),
		squareVertexBuffer_(createSquareVertexBuffer(renderer_)),
		squareIndexBuffer_(createSquareIndexBuffer(renderer_))
	{
	}

	void setDefaultRenderingParameters() {
		auto& commandList = renderer().getImmediateCommandList();

		commandList.setRenderTarget(renderer().backBuffer(), renderer().depthStencil());
		commandList.setRasteriser(rasteriser_);
		commandList.setViewport(viewport_);
	}

	void setSquareData() {
		auto& commandList = renderer().getImmediateCommandList();
		
		commandList.setVertexBuffer(squareVertexBuffer_, 0);
		commandList.setIndexBuffer(squareIndexBuffer_, 0);
	}

	void drawSquare() {
		renderer().getImmediateCommandList().draw(0, 4, PrimitiveTopology::TRIANGLE_STRIP);
	}

	Renderer& renderer() noexcept {
		return renderer_;
	}

	system::App& app() {
		return *app_;
	}

private:

	enum {
		WINDOW_WIDTH = 800,
		WINDOW_HEIGHT = 600
	};

	std::shared_ptr<system::App> app_;

	system::Window window_;

	Renderer renderer_;

	Rasteriser rasteriser_;

	Viewport viewport_;

	VertexBuffer squareVertexBuffer_;

	IndexBuffer squareIndexBuffer_;

	static system::Window::Configuration windowConfiguration() {
		system::Window::Configuration conf;
		conf.className = "GraphicsTestSuiteWindow";
		conf.fullscreen = false;
		conf.height = WINDOW_HEIGHT;
		conf.width = WINDOW_WIDTH;
		conf.title = "GraphicsTestSuite window";
		return conf;
	}

	static Renderer::Configuration rendererConfiguration() {
		Renderer::Configuration conf;
		conf.debugDevice = true;
		conf.sampleCount = 1;
		conf.sampleQuality = 0;
		conf.vsync = true;
		return conf;
	}

	static Rasteriser::Configuration rasteriserConfiguration() {
		Rasteriser::Configuration conf;
		conf.cullMode = Rasteriser::CullMode::BACK;
		conf.fillMode = Rasteriser::FillMode::SOLID;
		conf.frontCounterClockwise = false;
		return conf;
	}

	static Viewport::Configuration viewportConfiguration() {
		Viewport::Configuration conf;
		conf.height = WINDOW_HEIGHT;
		conf.width = WINDOW_WIDTH;
		conf.topLeftX = 0.0f;
		conf.topLeftY = 0.0f;
		conf.minDepth = 0.0f;
		conf.maxDepth = 1.0f;
		return conf;
	}

	static VertexBuffer createSquareVertexBuffer(Renderer& renderer) {
		const std::array<float, 4 * 4> data = {
			-1.0f, +1.0f, 0.0f, +1.0f,
			+1.0f, +1.0f, 0.0f, +1.0f,
			-1.0f, -1.0f, 0.0f, +1.0f,
			+1.0f, -1.0f, 0.0f, +1.0f
			};

		Buffer::Configuration conf;
		conf.allowCPURead = false;
		conf.allowGPUWrite = false;
		conf.allowModifications = false;
		conf.size = data.size() * sizeof(data[0]);
		conf.stride = sizeof(data[0]) * 4;

		return VertexBuffer(renderer, conf, data.data());
	}

	static IndexBuffer createSquareIndexBuffer(Renderer& renderer) {
		const std::array<std::uint32_t, 4> data = { 0, 1, 2, 3 };

		Buffer::Configuration conf;
		conf.allowCPURead = false;
		conf.allowGPUWrite = false;
		conf.allowModifications = false;
		conf.size = data.size() * sizeof(data[0]);
		conf.stride = sizeof(data[0]);

		return IndexBuffer(renderer, conf, data.data());
	}

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_TESTSUITE_HPP_ */
