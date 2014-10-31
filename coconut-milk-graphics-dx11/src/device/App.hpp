#ifndef _COCONUT_MILK_GRAPHICS_DX11_DEVICE_APP_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_DEVICE_APP_HPP_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Window.hpp"

namespace coconut {
namespace milk {
namespace graphics {
namespace device {

class App {
public:

	App(HINSTANCE instance, LPSTR commandLine, int showCommand);

	void update();

	bool closeRequested() const {
		return closeRequested_;
	}

	HINSTANCE instance() {
		return instance_;
	}

	LRESULT systemCallback(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
		return DefWindowProc(window, message, wparam, lparam);
	}

private:

	HINSTANCE instance_;

	LPSTR commandLine_;

	int showCommand_;

	bool closeRequested_;

};

} // namespace device
} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_DEVICE_APP_HPP_ */
