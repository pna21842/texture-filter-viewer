
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "PrincipleAxesModel.h"

using namespace std;
using namespace cst;

// global variables

// Example texture object
GLuint playerTexture;

ArcballCamera* mainCamera = nullptr;
bool mouseDown = false;
double prevMouseX, prevMouseY;

PrincipleAxesModel* principleAxes = nullptr;




// Window size
const unsigned int initWidth = 512;
const unsigned int initHeight = 512;

// Function prototypes
void renderScene();
void mouseMoveHandler(GLFWwindow* window, double xpos, double ypos);
void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods);
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

	principleAxes->render(T);

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
		cout << "x = " << xpos << ", y = " << ypos << endl;
	}

}



void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT) {

		if (action == GLFW_PRESS) {

			cout << "Left button press" << endl;
			mouseDown = true;
		}
		else if (action == GLFW_RELEASE) {

			cout << "Left button release" << endl;
			mouseDown = false;
		}
	}
}


void mouseEnterHandler(GLFWwindow* window, int entered) {

	if (entered) {

		// The cursor entered the content area of the window
		cout << "Enter window" << endl;
	}
	else {

		// The cursor left the content area of the window
		cout << "Exit window" << endl;
	}
}


// Function to call when window resized
void resizeWindow(GLFWwindow* window, int width, int height)
{
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

