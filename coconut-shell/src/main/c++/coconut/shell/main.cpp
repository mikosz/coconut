#include <memory>
#include <exception>
#include <iostream>

#include <coconut-tools/logger.hpp>

#include "coconut/milk/system/App.hpp"

#include "game/Game.hpp"

using namespace coconut;
using namespace coconut::shell;

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.OBJ_MODEL_PARSER");

CCN_BEGIN_MAIN();
	auto app = std::make_shared<milk::system::App>(*milk::system::GlobalMainArguments::instance());
	game::Game theGame(app);
	theGame.loop();

	return 0;
CCN_END_MAIN();
