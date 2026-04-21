#include "Game.h"

const char* GAME_NAME = "Nondescript Voxel Game";

int main() {
	Game game(GAME_NAME, SCREEN_HEIGHT, SCREEN_WIDTH);
	game.run();
	return 0;
}

