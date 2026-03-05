#include "Game.h"

const char* GAME_NAME = "Nondescript Voxel Game";

int main(int argc, char* argv[]) {
	Game game(GAME_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);
	game.run();
	return 0;
}

