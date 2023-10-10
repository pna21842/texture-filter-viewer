
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "PrincipleAxesModel.h"
#include "TexturedQuadModel.h"


using namespace std;
using namespace cst;

// global variables

// Example texture object
GLuint playerTexture;

// Camera model and tracking
ArcballCamera* mainCamera = nullptr;
bool mouseDown = false;
double prevMouseX, prevMouseY;

// Demo object
PrincipleAxesModel* principleAxes = nullptr;

// Road textures
static const GLuint NUM_ROADS = 5;

TexturedQuadModel* road[NUM_ROADS];
int currentRoad;


// Window size
const unsigned int initWidth = 512;
const unsigned int initHeight = 512;

// Function prototypes
void renderScene();
void mouseMoveHandler(GLFWwindow* window, double xpos, double ypos);
void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods);
void mouseScrollHandler(GLFWwindow* window, double xoffset, double yoffset);
void mouseEnterHandler(GLFWwindow* window, int entered);
void resizeWindow(GLFWwindow* window, int width, int height);
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void updateScene();


int main() {

	//
	// 1. Initialisation
	//
	

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "Real-Time Computer Graphics", NULL, NULL);

	// Check window was created successfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	

	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, resizeWindow); // resize window callback
	glfwSetKeyCallback(window, keyboardHandler); // Keyboard input callback
	glfwSetCursorPosCallback(window, mouseMoveHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetScrollCallback(window, mouseScrollHandler);
	glfwSetCursorEnterCallback(window, mouseEnterHandler);

	// Initialise glew
	glewInit();

	
	// Setup window's initial size
	resizeWindow(window, initWidth, initHeight);

	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black


	//
	// Setup textures
	//

	// Load image file from disk
	playerTexture = fiLoadTexture(
		string("Assets\\Textures\\player1_ship.png"),
		FIF_PNG,
		TextureProperties(
			GL_RGBA,
			GL_NEAREST,
			GL_NEAREST,
			1.0f,
			GL_REPEAT,
			GL_REPEAT,
			false,
			true)
	);
	
	// Setup main camera
	float viewportAspect = (float)initWidth / (float)initHeight;
	mainCamera = new ArcballCamera(0.0f, 0.0f, 5.0f, 55.0f, viewportAspect, 0.1f, 1000.0f);

	principleAxes = new PrincipleAxesModel();


	//
	// Load example road texture with different filtering properites
	//

	// Point filtering
	road[0] = new TexturedQuadModel(string("Assets\\Textures\\road.bmp"), FIF_BMP, TextureProperties(GL_COMPRESSED_SRGB, GL_NEAREST, GL_NEAREST, 1.0f, GL_REPEAT, GL_REPEAT, false, true));

	// Bilinear filtering
	road[1] = new TexturedQuadModel(string("Assets\\Textures\\road.bmp"), FIF_BMP, TextureProperties(GL_SRGB8_ALPHA8, GL_LINEAR, GL_LINEAR, 1.0f, GL_REPEAT, GL_REPEAT, false, true));

	// Tri-linear filtering
	road[2] = new TexturedQuadModel(string("Assets\\Textures\\road.bmp"), FIF_BMP, TextureProperties(GL_SRGB8_ALPHA8, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1.0f, GL_REPEAT, GL_REPEAT, true, true));

	// Anisotropic x2
	road[3] = new TexturedQuadModel(string("Assets\\Textures\\road.bmp"), FIF_BMP, TextureProperties(GL_SRGB8_ALPHA8, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 2.0f, GL_REPEAT, GL_REPEAT, true, true));

	// Anisotropic x8
	road[4] = new TexturedQuadModel(string("Assets\\Textures\\road.bmp"), FIF_BMP, TextureProperties(GL_SRGB8_ALPHA8, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 8.0f, GL_REPEAT, GL_REPEAT, true, true));

	currentRoad = 0;


	//
	// 2. Main loop
	// 
	

	// Loop while program is not terminated.
	while (!glfwWindowShouldClose(window)) {

		updateScene();
		renderScene();						// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		// Poll events (key presses, mouse events)
		// glfwWaitEvents();				// Use this if no animation.
		// glfwWaitEventsTimeout(1.0/60.0);	// Use this to animate at 60 frames/sec (timing is NOT reliable)
		glfwPollEvents();					// Use this version when animating as fast as possible
	}

	glfwTerminate();
	return 0;
}



// renderScene - function to render the current scene
void renderScene()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get view-projection transform
	glm::mat4 T = mainCamera->projectionTransform() * mainCamera->viewTransform();

	glm::mat4 roadMVP =
		T *
		glm::rotate(glm::mat4(1.0f), glm::radians<float>(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(32.0f, 128.0f, 1.0f));

	road[currentRoad]->render(roadMVP);

	//principleAxes->render(T);

	// Render objects here...
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, playerTexture);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-0.5f, 0.5f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(0.5f, 0.5f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(0.5f, -0.5f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-0.5f, -0.5f);

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);*/
}



void mouseMoveHandler(GLFWwindow* window, double xpos, double ypos) {

	if (mouseDown) {

		//cout << "x = " << xpos << ", y = " << ypos << endl;
		double dx = xpos - prevMouseX;
		double dy = ypos - prevMouseY;

		if (mainCamera)
			mainCamera->rotateCamera((float)-dy, (float)-dx);

		prevMouseX = xpos;
		prevMouseY = ypos;
	}

}

void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT) {

		if (action == GLFW_PRESS) {

			mouseDown = true;
			glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
		}
		else if (action == GLFW_RELEASE) {

			mouseDown = false;
		}
	}
}

void mouseScrollHandler(GLFWwindow* window, double xoffset, double yoffset) {

	if (mainCamera) {

		if (yoffset < 0.0)
			mainCamera->scaleRadius(1.1f);
		else if (yoffset > 0.0)
			mainCamera->scaleRadius(0.9f);
	}
}

void mouseEnterHandler(GLFWwindow* window, int entered) {
}


// Function to call when window resized
void resizeWindow(GLFWwindow* window, int width, int height)
{
	if (mainCamera) {

		mainCamera->setAspectRatio((float)width / (float)height);
	}

	glViewport(0, 0, width, height);		// Draw into entire window
}


// Function to call to handle keyboard input
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {

		// check which key was pressed...
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break;

			case GLFW_KEY_1:
				currentRoad = 0;
				break;

			case GLFW_KEY_2:
				currentRoad = 1;
				break;

			case GLFW_KEY_3:
				currentRoad = 2;
				break;

			case GLFW_KEY_4:
				currentRoad = 3;
				break;

			case GLFW_KEY_5:
				currentRoad = 4;
				break;

			default:
			{
			}
		}
	}
	else if (action == GLFW_RELEASE) {
		// handle key release events
	}
}


// Function called to animate elements in the scene
void updateScene() {
}

