#include "App.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::system;

App::App(const MainArguments& mainArguments) :
	instance_(mainArguments.hinstance),
	commandLine_(mainArguments.commandLine),
	showCommand_(mainArguments.showCommand),
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
