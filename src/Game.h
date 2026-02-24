#pragma once
#include "Texture.h"
#include "GLFW/glfw3.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;



class Game {
public:
	Game(const char* windowName, const int windowHeight, const int windowWidth);
	~Game();
	void run();
	static Texture* texture;
private:
	GLFWwindow* window;

	void processInput(GLFWwindow* window);
};
