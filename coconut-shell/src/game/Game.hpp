#ifndef _COCONUT_SHELL_GAME_GAME_HPP_
#define _COCONUT_SHELL_GAME_GAME_HPP_

#include "system/App.hpp"
#include "graphics/Device.hpp"

namespace coconut {
namespace shell {
namespace game {
			
class Game {
public:

	Game(std::shared_ptr<milk::system::App> app);

	void loop();

private:

	std::shared_ptr<milk::system::App> app_;

	std::shared_ptr<milk::system::Window> window_;

	std::shared_ptr<milk::graphics::Device> graphicsDevice_;

};

} // namespace game
} // namespace shell
} // namespace coconut

#endif /* _COCONUT_SHELL_GAME_GAME_HPP_ */
