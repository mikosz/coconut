#ifndef _COCONUT_MILK_SYSTEM_WIN32_APP_HPP_
#define _COCONUT_MILK_SYSTEM_WIN32_APP_HPP_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/singleton.hpp>
#include <coconut-tools/concurrent/fake.hpp>
#include <coconut-tools/policy/creation/New.hpp>

#include "Window.hpp"

namespace coconut {
namespace milk {
namespace system {

// TODO: extract to separate file
struct MainArguments {

	int argc;

	const char** argv;

	HINSTANCE hinstance;

	LPSTR commandLine;

	int showCommand;

	MainArguments() { // TODO: Singleton won't compile otherwise
	}

	MainArguments(int argc, const char** argv, HINSTANCE hinstance, LPSTR commandLine, int showCommand) :
		argc(argc),
		argv(argv),
		hinstance(hinstance),
		commandLine(commandLine),
		showCommand(showCommand)
	{
	}

};

struct GlobalMainArguments :
	public MainArguments,
	public coconut_tools::Singleton<
		MainArguments,
		coconut_tools::concurrent::FakeMutex,
		coconut_tools::policy::creation::New<MainArguments> // TODO: replace with None
		>
{
};

class App {
public:

	App(const MainArguments& mainArguments);

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

} // namespace system
} // namespace milk
} // namespace coconut

#define CCN_BEGIN_MAIN() \
	int CALLBACK WinMain(HINSTANCE instance__, HINSTANCE, LPSTR cmdLine__, int cmdShow__) { \
		{ \
			coconut::milk::system::GlobalMainArguments::setInstance( \
				std::make_unique<coconut::milk::system::MainArguments>( \
					__argc, const_cast<const char**>(__argv), instance__, cmdLine__, cmdShow__) \
				); \
		}

#define CCN_END_MAIN() }

#endif /* _COCONUT_MILK_SYSTEM_WIN32_APP_HPP_ */
