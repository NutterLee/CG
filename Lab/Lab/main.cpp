// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Drone.h"
#include "Human.h"
#include "Mortar.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "../SOIL2/SOIL2.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(10.0f, 15.10f, 20.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
//todo 利用deltaTime来设置时间间隔，只有时间间隔到达一定的程度才会进行调整加速度，速度什么的
GLfloat calculateTime = 0.0f;
bool droneState = true;
bool droneHasFound = false;

//Light attributes
glm::vec3 lightPos(1.2f, 8.0f, 2.0f);

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CG", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(10,10, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);


	// Setup and compile our shaders
	Shader shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");
	Shader droneShader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");
	Shader humanShader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");
	Shader mortarShader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");

	// Load models
	Model house("res/models/env/Street_environment.obj");
	Drone drone;
	drone.loadModel("res/models/drone/Drone.obj");
	drone.setPos(-5, 5, -5);

	Human human;
	human.loadModel("res/models/human/man.obj");
	human.setPos(0, 0, 0);

	Mortar mortar;
	mortar.loadModel("res/models/mortar/supermortar.obj");
	mortar.setPos(0.0, 0, 7.0);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 200.0f);
	
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		//cout << "delta Time: " << deltaTime << endl;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//draw env
		shader.Use();

		GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(shader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(shader.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		GLint modelLoc = glGetUniformLocation(shader.Program, "model");

		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.9f, 0.59f, 0.9f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		house.Draw(shader);

		//draw drone
		droneShader.Use();

		GLint objectColorLoc_drone = glGetUniformLocation(droneShader.Program, "objectColor");
		GLint lightColorLoc_drone = glGetUniformLocation(droneShader.Program, "lightColor");
		GLint lightPosLoc_drone = glGetUniformLocation(droneShader.Program, "lightPos");
		GLint viewPosLoc_drone = glGetUniformLocation(droneShader.Program, "viewPos");
		glUniform3f(objectColorLoc_drone, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightColorLoc_drone, 0.8f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc_drone, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc_drone, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		GLint modelLoc_drone = glGetUniformLocation(droneShader.Program, "model");

		glm::mat4 view_drone = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(droneShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(droneShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view_drone));

		// Draw the loaded model
		glm::mat4 model_drone;
		//todo 此处的运动逻辑还需要修改
		GLdouble distance = sqrt((drone.getPosX() - human.getPosX())*(drone.getPosX() - human.getPosX())
			+ (drone.getPosY() - human.getPosY()-3.0)*(drone.getPosY() - human.getPosY()-3.0)
			+ (drone.getPosZ() - human.getPosZ())*(drone.getPosZ() - human.getPosZ()));
	//	cout << "distance: " << distance << endl;
		if (droneState == false) {
			//cout << "falling down" << endl;
			drone.falldown();
		}
		else if (drone.hasFound(human.getPosX(),human.getPosY(),human.getPosZ())||droneHasFound) {
			//cout << "found target!"<<"dis in found "<<distance << endl;
			droneHasFound = true;
			drone.flyToPos(human.getPosX(), human.getPosY()+3.0, human.getPosZ(), 0);
			if (abs(distance) < 0.5)
				droneState = false;			
		}
		else {
			//cout << "hit here!" << endl;
			drone.searchInArea(-6.0f, 3.5f, -6.0f, 7.0f, 4.0f, 7.0f);			
		}
		//drone.falldown();
		model_drone = glm::translate(model_drone, glm::vec3(drone.getPosX(), drone.getPosY(), drone.getPosZ())); // Translate it down a bit so it's at the center of the scene
		//model_drone = glm::translate(model_drone, glm::vec3(0,3,0));
		model_drone = glm::scale(model_drone, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(droneShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_drone));
		drone.draw(droneShader);

		//draw  human
		humanShader.Use();

		GLint objectColorLoc_human = glGetUniformLocation(humanShader.Program, "objectColor");
		GLint lightColorLoc_human = glGetUniformLocation(humanShader.Program, "lightColor");
		GLint lightPosLoc_human = glGetUniformLocation(humanShader.Program, "lightPos");
		GLint viewPosLoc_human = glGetUniformLocation(humanShader.Program, "viewPos");
		glUniform3f(objectColorLoc_human, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightColorLoc_human, 0.8f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc_human, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc_human, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		GLint modelLoc_human = glGetUniformLocation(humanShader.Program, "model");

		glm::mat4 view_human = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(humanShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(humanShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view_human));

		// Draw the loaded model
		glm::mat4 model_human;
		//human.randomMoveWithLimit(0, 0, 4, 4);
		//human.randomMoveToPoint(-4, 4);
		human.staticMove();
		
		model_human = glm::translate(model_human, glm::vec3(human.getPosX(), human.getPosY(), human.getPosZ()));
		model_human = glm::rotate(model_human, human.getRotateAngle(), glm::vec3(0, 1,0));
		model_human = glm::translate(model_human, glm::vec3(0, 0, 0));
		model_human = glm::scale(model_human, glm::vec3(0.15f, 0.15f, 0.15f));	// It's a bit too big for our scene, so scale it down
		//model_human = glm::rotate(model_human, -45.228f, glm::vec3(0, 1, 0));

		
		glUniformMatrix4fv(glGetUniformLocation(humanShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_human));
		human.draw(humanShader);

		//draw mortar
		mortarShader.Use();

		GLint objectColorLoc_mortar = glGetUniformLocation(mortarShader.Program, "objectColor");
		GLint lightColorLoc_mortar = glGetUniformLocation(mortarShader.Program, "lightColor");
		GLint lightPosLoc_mortar = glGetUniformLocation(mortarShader.Program, "lightPos");
		GLint viewPosLoc_mortar = glGetUniformLocation(mortarShader.Program, "viewPos");
		glUniform3f(objectColorLoc_mortar, 1.0f, 0.3f, 0.31f);
		glUniform3f(lightColorLoc_mortar, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc_mortar, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc_mortar, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		GLint modelLoc_mortar = glGetUniformLocation(mortarShader.Program, "model");
		//glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(mortarShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(mortarShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model_mortar;
		mortar.rotateToTarget(drone.getPosX(), drone.getPosY(), drone.getPosZ());
		model_mortar = glm::translate(model_mortar, glm::vec3(mortar.getPosX(), mortar.getPosY(), mortar.getPosZ())); // Translate it down a bit so it's at the center of the scene
		//model_mortar = glm::translate(model_mortar, glm::vec3(0, 0, 0));
		//model_mortar = glm::rotate(model_mortar, -80.0f, glm::vec3(1, 0, 0));
		//model_mortar = glm::rotate(model_mortar, -120.0f, glm::vec3(0, 1, 0));
		model_mortar = glm::rotate(model_mortar, mortar.getRotateAngle(), glm::vec3(0, 1, 0));
		//model_mortar = glm::rotate(model_mortar,165.0f, glm::vec3(0, 0, 1));
		model_mortar = glm::translate(model_mortar, glm::vec3(0, 0, 0));
		model_mortar = glm::scale(model_mortar, glm::vec3(0.005f, 0.005f, 0.005f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(mortarShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_mortar));
		mortar.draw(mortarShader);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	system("pause");
	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		//system("pause");
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}


