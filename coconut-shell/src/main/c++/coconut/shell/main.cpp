#include <memory>
#include <exception>
#include <iostream>

#include <coconut-tools/logger.hpp>
#include <coconut-tools/configuration/readers/HierarchicalConfigurationReader.hpp>
//#include <coconut-tools/configuration/parsers/JSONParser.hpp>
#include <coconut-tools/configuration/parsers/XMLParser.hpp>

#include "coconut/milk/system/App.hpp"

#include "game/Game.hpp"

using namespace coconut;
using namespace coconut::shell;

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.OBJ_MODEL_PARSER");

//CT_BEGIN_MAIN();
int main() {
	try {
		auto app = std::make_shared<milk::system::App>(*milk::system::GlobalMainArguments::instance());

		auto config = coconut_tools::configuration::hierarchical::HierarchicalConfiguration::create();
		const auto configReader = coconut_tools::configuration::readers::HierarchicalConfigurationReader();
		//const auto configParser = coconut_tools::configuration::parsers::JSONParser();
		const auto configParser = coconut_tools::configuration::parsers::XMLParser();
		// TODO: JSON parser doesn't work :(((
		//configReader.read(configParser, "coconut-shell.cfg.json", config.get());
		configReader.read(configParser, "coconut-shell.cfg.xml", config.get());

		game::Game theGame(app, config);
		theGame.loop();

		return 0;
	} catch (const std::exception& e) {
		CT_LOG_CRITICAL << "Caught exception: " << e.what();
		return 1;
	}
}
//CT_END_MAIN();
