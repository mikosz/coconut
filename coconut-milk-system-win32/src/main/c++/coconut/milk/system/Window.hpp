#ifndef _COCONUT_MILK_SYSTEM_WIN32_WINDOW_HPP_
#define _COCONUT_MILK_SYSTEM_WIN32_WINDOW_HPP_

#include <memory>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

namespace coconut {
namespace milk {
namespace system {

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

	size_t clientWidth() const;

	size_t clientHeight() const;

	bool fullscreen() const;

	HWND handle() {
		return handle_;
	}

private:

	Configuration configuration_;

	std::shared_ptr<App> app_;

	HWND handle_;

	static LRESULT CALLBACK messageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

};

} // namespace system
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_SYSTEM_WIN32_WINDOW_HPP_ */
