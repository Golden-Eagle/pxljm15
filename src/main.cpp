#include <iostream>

#include "Assets.hpp"
#include "Game.hpp"


int main( void ) {

	std::cout << std::boolalpha;

	// randomly placed note about texture parameters and debug messages:
	// nvidia uses this as mipmap allocation hint; not doing it causes warning spam
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	gecom::assets::init("../AssetConfig");

	gecom::Game *game = new gecom::Game();
	game->run();




	return 0;
}
