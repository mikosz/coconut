#include "Window.hpp"

#include <cstring>
#include <stdexcept>
#include <sstream>

#include "App.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::system;

Window::Window(const Configuration& configuration, std::shared_ptr<App> app) :
	configuration_(configuration),
	app_(app),
	handle_(0)
{
	WNDCLASSEXA wndClassEx;
	std::memset(&wndClassEx, 0, sizeof(WNDCLASSEX));

	wndClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClassEx.lpfnWndProc = &messageHandler;
	wndClassEx.hInstance = app->instance();
	wndClassEx.hIcon = LoadIcon(0, IDI_WINLOGO);
	wndClassEx.hIconSm = wndClassEx.hIcon;
	wndClassEx.hCursor = LoadCursor(0, IDC_ARROW);
	wndClassEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wndClassEx.lpszClassName = configuration_.className.c_str();
	wndClassEx.cbSize = sizeof(WNDCLASSEX);

	if (RegisterClassExA(&wndClassEx) == 0) {
		std::ostringstream error;
		error << "Failed to register window class. Error code: " << GetLastError();

		throw std::runtime_error(error.str());
	}

	handle_ = CreateWindowExA(
		WS_EX_APPWINDOW,
		configuration_.className.c_str(),
		configuration_.title.c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		100,
		100,
		configuration_.width,
		configuration_.height,
		0,
		0,
		app_->instance(),
		this
		);

	if (handle_ == 0) {
		std::ostringstream error;
		error << "Failed to create window. Error code: " << GetLastError();

		throw std::runtime_error(error.str());
	}

	ShowWindow(handle_, SW_SHOW);
	SetForegroundWindow(handle_);
	SetFocus(handle_);

	ShowCursor(TRUE);
}

Window::~Window() {
	if (handle_ != 0) {
		DestroyWindow(handle_);
		handle_ = 0;

		UnregisterClassA(configuration_.className.c_str(), app_->instance());
	}
}

LRESULT CALLBACK Window::messageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	switch (message) {
	case WM_NCCREATE:
		{
			LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lparam);
			Window* instance = reinterpret_cast<Window*>(createStruct->lpCreateParams);

			if (!instance) {
				throw std::logic_error("Window instance create parameter shall not be null!");
			}

			::SetWindowLong(window, GWL_USERDATA, reinterpret_cast<LONG>(instance));

			return TRUE;
		}
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;
	default:
		{
			Window* instance = reinterpret_cast<Window*>(::GetWindowLong(window, GWL_USERDATA));

			if (!instance) {
				throw std::logic_error("Window instance shall not be null!");
			}

			return instance->app_->systemCallback(window, message, wparam, lparam);
		}
	}
}

size_t Window::clientWidth() const {
	RECT clientRect;
	if (!GetClientRect(handle_, &clientRect)) {
		throw std::runtime_error("Failed to retrieve client width");
	}

	return clientRect.right - clientRect.left;
}

size_t Window::clientHeight() const {
	RECT clientRect;
	if (!GetClientRect(handle_, &clientRect)) {
		throw std::runtime_error("Failed to retrieve client height");
	}

	return clientRect.bottom - clientRect.top;
}

bool Window::fullscreen() const {
	return configuration_.fullscreen;
}
