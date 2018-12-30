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
#include "Texture.h"
#include "Skybox.h"

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
	GLfloat cubeVertices[] =
	{
		// Positions          // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

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
	glViewport(10, 10, SCREEN_WIDTH, SCREEN_HEIGHT);

	
	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);


	// Setup and compile our shaders
	//Shader shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");
	Shader shader("res/shaders/droneShader.vs", "res/shaders/droneShader.frag");
	//Shader droneShader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");
	Shader droneShader("res/shaders/droneShader.vs", "res/shaders/droneShader.frag");
	Shader humanShader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");
	Shader mortarShader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");
	Shader skyBoxShader("res/shaders/skybox.vs", "res/shaders/skybox.frag");
	Shader shadowShader("res/shaders/shadow_mapping.vs", "res/shaders/shadow_mapping.frag");
	Shader simpleDepthShader("res/shaders/shadow_mapping_depth.vs", "res/shaders/shadow_mapping_depth.frag");


	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//configure shadow shader
	shadowShader.Use();
	shadowShader.setInt("diffuseTexture", 0);
	shadowShader.setInt("shadowMap", 1);


	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glBindVertexArray(0);
		
	std::vector<const char*> faces;
	//faces.push_back("res/images/hw_glacier/glacier_rt.tga");
	//faces.push_back("res/images/hw_glacier/glacier_lf.tga");
	//faces.push_back("res/images/hw_glacier/glacier_up.tga");
	//faces.push_back("res/images/hw_glacier/glacier_dn.tga");
	//faces.push_back("res/images/hw_glacier/glacier_bk.tga");
	//faces.push_back("res/images/hw_glacier/glacier_ft.tga");
	faces.push_back("res/images/mp_deviltooth/devils-tooth_rt.tga");
	faces.push_back("res/images/mp_deviltooth/devils-tooth_lf.tga");
	faces.push_back("res/images/mp_deviltooth/devils-tooth_up.tga");
	faces.push_back("res/images/mp_deviltooth/devils-tooth_dn.tga");
	faces.push_back("res/images/mp_deviltooth/devils-tooth_bk.tga");
	faces.push_back("res/images/mp_deviltooth/devils-tooth_ft.tga");
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);


	// Load models
	//Model house("res/models/env/Street_environment.obj");
	Model house("res/models/Small Tropical Island/Small Tropical Island.obj");
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

		/*todo!!!!
		//
		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader.Use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		//todo render model here use simple depth shader ???
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//reset viewport
		glViewport(10, 10, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 */
		//----------------
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
		model = glm::translate(model, glm::vec3(15.0f, -6.75f, -16.0f)); // Translate it down a bit so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.9f, 0.59f, 0.9f));	// It's a bit too big for our scene, so scale it down
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));	// It's a bit too big for our scene, so scale it down
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
			+ (drone.getPosY() - human.getPosY() - 3.0)*(drone.getPosY() - human.getPosY() - 3.0)
			+ (drone.getPosZ() - human.getPosZ())*(drone.getPosZ() - human.getPosZ()));
		//	cout << "distance: " << distance << endl;
		if (droneState == false) {
			//cout << "falling down" << endl;
			drone.falldown();
		}
		else if (drone.hasFound(human.getPosX(), human.getPosY(), human.getPosZ()) || droneHasFound) {
			//cout << "found target!"<<"dis in found "<<distance << endl;
			droneHasFound = true;
			drone.flyToPos(human.getPosX(), human.getPosY() + 3.0, human.getPosZ(), 0);
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
		model_human = glm::rotate(model_human, human.getRotateAngle(), glm::vec3(0, 1, 0));
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


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		skyBoxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix

		glUniformMatrix4fv(glGetUniformLocation(skyBoxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyBoxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default


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


