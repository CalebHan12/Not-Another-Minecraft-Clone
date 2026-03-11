#include <iostream>

#include "Camera.h"
#include "Chunk.h"
#include "Game.h"
#include "WorldManager.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

Texture* Game::texture = nullptr;

float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// time between current frame  and last frame
float lastFrame = 0.0f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	(void)window; // unused
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
	(void)window; // unused

	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if(!game)
		return;

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if(firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	game->player.getCamera().ProcessMouseMovement(xoffset, yoffset);
}

Game::Game(const char* windowName, const int windowHeight, const int windowWidth) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowHeight, windowWidth, windowName, NULL, NULL);

	// Creating stuff in a ctor is probably a bad idea
	// TODO: Move to seperate function with proper error handling, maybe throw an exception?
	if(window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); //  Disable v-sync
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, mouseCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load OpenGL function pointers
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	// Enable the Z-buffer
	glEnable(GL_DEPTH_TEST);

	// TODO: ... the entire texture system lol.
	Game::texture = new Texture("textures/texture_atlas.jpg");
}

void Game::processInput(GLFWwindow* inputWindow) {
	Camera& camera = this->player.getCamera();
	if(glfwGetKey(inputWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(inputWindow, true);

	if(glfwGetKey(inputWindow, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	if(glfwGetKey(inputWindow, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	if(glfwGetKey(inputWindow, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	if(glfwGetKey(inputWindow, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
}


void Game::run() {
	WorldManager wm;

	while(!glfwWindowShouldClose(window)) {
		// Compute delta time
		auto currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Gather input
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		wm.updatePosition(player.getCamera().Position);
		wm.render(player.getCamera());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
