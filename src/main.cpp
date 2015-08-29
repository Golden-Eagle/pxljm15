#include <iostream>

#include "Assets.hpp"
#include "Game.hpp"
#include "PlayState.hpp"


int main( void ) {

	std::cout << std::boolalpha;

	// randomly placed note about texture parameters and debug messages:
	// nvidia uses this as mipmap allocation hint; not doing it causes warning spam
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	pxljm::Game game;
	game.init<pxljm::PlayState>();
	game.run();




	return 0;
}
