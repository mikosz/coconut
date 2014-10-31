#ifndef _COCONUT_MILK_GRAPHICS_DX11_DEVICE_WINDOW_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_DEVICE_WINDOW_HPP_

#include <memory>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace coconut {
namespace milk {
namespace graphics {
namespace device {

class App;

class Window {
public:

	struct Configuration {

		size_t width;

		size_t height;

		bool fullscreen;

		std::string className;

		std::string title;

	};

	Window(const Configuration& configuration, std::shared_ptr<App> app);

	~Window();

private:

	Configuration configuration_;

	std::shared_ptr<App> app_;

	HWND handle_;

	static LRESULT CALLBACK messageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

};

} // namespace device
} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_DEVICE_WINDOW_HPP_ */
