#include <memory>

#include "milk/system/App.hpp"
#include "game/Game.hpp"

#pragma comment(lib, "coconut-pulp-renderer.lib")
#pragma comment(lib, "coconut-milk-graphics-dx11.lib")
#pragma comment(lib, "coconut-milk-math-xna.lib")
#pragma comment(lib, "coconut-milk-system-win32.lib")

using namespace coconut;
using namespace coconut::shell;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	std::shared_ptr<milk::system::App> app(new milk::system::App(hInstance, pScmdline, iCmdshow));

	game::Game theGame(app);
	theGame.loop();

	return 0;
}
