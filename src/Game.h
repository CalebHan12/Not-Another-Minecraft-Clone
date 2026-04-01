#pragma once
#include "Texture.h"
#include "GLFW/glfw3.h"
#include "Player.h"
#include "WorldManager.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

class Game {
public:
	Game(const char* windowName, const int windowHeight, const int windowWidth);
	~Game() { glfwTerminate(); };

	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;

	Game(Game&& other) noexcept = delete;
	Game& operator=(Game&& other) noexcept = delete;

	void run();
	static Texture* texture;

	Player player;
	WorldManager wm;
private:
	GLFWwindow* window;
	void processInput(GLFWwindow* window);
	void processLeftClick();
};
