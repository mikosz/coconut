#ifndef _COCONUT_SHELL_GAME_GAME_HPP_
#define _COCONUT_SHELL_GAME_GAME_HPP_

#include "device/App.hpp"

namespace coconut {
namespace shell {
namespace game {
			
class Game {
public:

	Game(std::shared_ptr<milk::graphics::device::App> app);

	void loop();

private:

	std::shared_ptr<milk::graphics::device::App> app_;

};

} // namespace game
} // namespace shell
} // namespace coconut

#endif /* _COCONUT_SHELL_GAME_GAME_HPP_ */
