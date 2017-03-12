#ifndef _COCONUT_SHELL_GAME_GAME_HPP_
#define _COCONUT_SHELL_GAME_GAME_HPP_

#include <memory>

#include <windows.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/App.hpp"
#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/fs/Filesystem.hpp"

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

	std::shared_ptr<milk::fs::Filesystem> filesystem_;

	std::shared_ptr<milk::graphics::Renderer> graphicsRenderer_;

};

} // namespace game
} // namespace shell
} // namespace coconut

#endif /* _COCONUT_SHELL_GAME_GAME_HPP_ */
