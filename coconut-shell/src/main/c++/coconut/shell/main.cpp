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

CCN_BEGIN_MAIN();
	// try {
		auto app = std::make_shared<milk::system::App>(*milk::system::GlobalMainArguments::instance());
		game::Game theGame(app);
		theGame.loop();
	/* } catch (const std::exception& e) {
		CT_LOG_CRITICAL << "Caught exception: " << e.what() << '\n';
	}*/

	return 0;
CCN_END_MAIN();
