#include "Game.h"
#include "Chunk.h"
#include "Camera.h"

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// TODO: Move this stuff out of global space
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Texture* Game::texture = nullptr;

float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// time between current frame  and last frame
float lastFrame = 0.0f;

void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* /*window*/, double xposIn, double yposIn) {
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

Game::Game(const char* windowName, const int windowHeight, const int windowWidth) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowHeight, windowWidth, windowName, NULL, NULL);
	
	// Creating stuff in a ctor is probably a bad idea
	// TODO: Move to seperate function with proper error handling
	if(window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); //  Disable v-sync
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
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

Game::~Game() {
	glfwTerminate();
}

void Game::processInput(GLFWwindow* inputWindow) {
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
	ChunkLayers chunkLayers;

	Layer l;
	for (int z = 0; z < LayerSize; ++z) {
		l.fill(BlockType::dirt);
		chunkLayers[z] = l;
	}

	Chunk chunk(chunkLayers);
	glm::vec3 pos(0, 0, 0);
	while (!glfwWindowShouldClose(window)) {
		// Compute delta time
		auto currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);
		
		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);		
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		chunk.draw(pos, camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
