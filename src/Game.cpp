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
	this->wm.shader.loadShader("shaders/shader.vert", "shaders/shader.frag");
}

void Game::processInput(GLFWwindow* window) {
	Camera& camera = this->player.getCamera();
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		processLeftClick();
}

void Game::run() {
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

void Game::processLeftClick()
{
	glm::vec3 position = player.getCamera().Position;
	glm::vec3 looking = player.getCamera().Front;
	int maxLength = 10;

	int stepX = looking.x <= 0 ? -1 : 1;
	int stepY = looking.y <= 0 ? -1 : 1;
	int stepZ = looking.z <= 0 ? -1 : 1;
	for(int i = 0; i < maxLength; ++i) {
		int x = position.x + (i * stepX);
		int y = position.y + (i * stepY);
		int z = position.z + (i * stepZ);
		BlockType block = wm.getBlock(x, y, z);
		if(block != BlockType::air) {
			std::cout << "Found a block: " << static_cast<int>(block) << "\n";
			return;
		}
	}
}