#include "device/App.hpp"

#include <memory>

#include "game/Game.hpp"

#pragma comment(lib, "coconut-milk-graphics-dx11.lib")

using namespace coconut;
using namespace coconut::shell;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	std::shared_ptr<milk::graphics::device::App> app(new milk::graphics::device::App(hInstance, pScmdline, iCmdshow));

	game::Game theGame(app);
	theGame.loop();

	return 0;
}
