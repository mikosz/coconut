#include <memory>
#include <exception>
#include <iostream>

#include <coconut-tools/logger.hpp>

#include "coconut/milk/system/App.hpp"

#include "game/Game.hpp"

#pragma comment(lib, "coconut-pulp-renderer.lib")
#pragma comment(lib, "coconut-milk-graphics-dx11.lib")
#pragma comment(lib, "coconut-milk-math-xna.lib")
#pragma comment(lib, "coconut-milk-system-win32.lib")

using namespace coconut;
using namespace coconut::shell;

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.OBJ_MODEL_PARSER");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	try {
		std::shared_ptr<milk::system::App> app(new milk::system::App(hInstance, pScmdline, iCmdshow));

		game::Game theGame(app);
		theGame.loop();
	} catch (const std::exception& e) {
		CT_LOG_CRITICAL << "Caught exception: " << e.what() << '\n';
	}

	return 0;
}
