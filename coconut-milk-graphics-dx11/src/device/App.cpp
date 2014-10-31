#include "App.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;
using namespace coconut::milk::graphics::device;

App::App(HINSTANCE instance, LPSTR commandLine, int showCommand) :
	instance_(instance),
	commandLine_(commandLine),
	showCommand_(showCommand),
	closeRequested_(false)
{
}

void App::update() {
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT) {
			closeRequested_ = true;
		}

		continue;
	}
}
