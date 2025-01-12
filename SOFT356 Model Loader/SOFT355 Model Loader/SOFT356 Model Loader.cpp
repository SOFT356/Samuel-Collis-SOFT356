
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp> //includes GLM
#include <string>
#include "ModelLoader.h"
#include "ShaderLoader.h"

GLfloat move = 1.0f;
GLfloat scaleIncrament = 0.005f;
glm::vec3 cameraLoc = glm::vec3(0.0f, 0.0f, 0.0f);
bool removeItem = false;

//A simple key callback function
void singleCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		removeItem = true;
	}		
}

//Method for dealing with user inputs
void processKeyEvents(GLFWwindow* window, glm::vec3& rotation, GLfloat& scale, bool& removeItem) {

	//If wasd is press then rotate the object via the x and y axis
	//if ijkl is pressed then move the camera
	//if uo are pressed then change the z axis of the camera
	//if -+ are pressed change the scale
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		cameraLoc.y -= 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		cameraLoc.y += 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		cameraLoc.x += 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		cameraLoc.x -= 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		cameraLoc.z += 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		cameraLoc.z -= 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		rotation.y -= move;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rotation.y += move;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		rotation.x -= move;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		rotation.x += move;
	}

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		scale += scaleIncrament;
	}

	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		scale -= scaleIncrament;
	}

	//If f is held then we want to show wireframe mode
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (scale < 0) {
		scale = 0.0000001;
	}
}


int main()
{
	Model model;

	//Variable for storing the users input
	std::string userInput;

	//Temp variables for creation
	//Model model;
	glm::vec3 location;

	//Stores for the models and locations
	std::vector<Model> models;
	std::vector<glm::vec3> modelLocations;

	std::cout << "Please enter a location you would like to place the camera in the form: x y z\nAny non parsable format will result in the camera being placed at 0,0,0" << std::endl;
	std::getline(std::cin, userInput);
	sscanf_s(userInput.c_str(), "%f %f %f", &cameraLoc.x, &cameraLoc.y, &cameraLoc.z);

	//do while for running the entire program
	do {
		//Do while for loading the objects
		do {

			std::cout << "Please input the file location of the object or enter \"quit\" to close.\n";

			//If we have more than one model then we are able to render the scene.
			if (models.size() > 0) {
				std::cout << "You can also type \"render\" to begin to render your scene.\n";
			}
			std::getline(std::cin, userInput);

			if (userInput._Equal("quit")) {
				return 0;
			}
			else if (userInput._Equal("render") && models.size() > 0) {
				break;
			}
			else if (userInput._Equal("render") && models.size() == 0) {
				std::cout << "We have safe guarded against empty scene rendering," <<
					"\nplease add at least one model and location." << std::endl;
				continue;
			}

			model = loadFromFile(userInput);

			if (!model.createdSuccessfully) {
				std::cout << "There was an issue loading your file, please try another" << std::endl;
				continue;
			}

			models.push_back(model);

			do {
				std::cout << "Please input Location you would like to render the object at in the format 'x y z'\n";
				std::getline(std::cin, userInput);

				if (sscanf_s(userInput.c_str(), "%f %f %f\n", &location.x, &location.y, &location.z) == 3) {
					modelLocations.push_back(location);
					break;
				}
				else {
					std::cout << "We were unable to parse the input, please try agian\n";
				}
			} while (true);
		
		} while (true);

		//Initialise glfw
		glfwInit();

		//Create our window
		GLFWwindow* window = glfwCreateWindow(800, 600, "Object Loader", NULL, NULL);

		//Initialise glew
		glfwMakeContextCurrent(window);
		glewInit();

		//Assign the key callback function
		//this function will only be called once for a keypress and release
		//otherwise we will process a keypress multiple times
		//which is fine for rotation but for removing would cause several items to be deleted at once
		glfwSetKeyCallback(window, singleCallback);

		
		//Initialise every loaded model
		for (int i = 0; i < models.size(); i++) {
			models[i].init();
		}

		//Set up values for rotation and scale
		static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		GLfloat scale = 1.0f;
		
		std::cout << "You are able to close the window with the Q key" << std::endl;
		
		glEnable(GL_DEPTH_TEST);
		
		//main opengl draw loop
		do {

			//Foreach model we want to rotate/translate/scale/draw it
			for (int i = 0; i < models.size(); i++) {
				models[i].translate(modelLocations[i]);
				models[i].rotate(rotation);
				models[i].scale(scale);
				models[i].setCameraLocation(cameraLoc);
				models[i].draw();
			}

			//We want to process user inputs
			processKeyEvents(window, rotation, scale, removeItem);

			glfwSwapBuffers(window);
			glfwPollEvents();

			//Then clear ready for next pass
			glClearBufferfv(GL_COLOR, 0, black);
			glClearColor(0.0f, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (removeItem && models.size() > 0) {
				models[models.size() - 1].destroy();
				models.pop_back();
				removeItem = false;
				std::cout << "Model removed" << std::endl;
			}
			
		} // Check if the ESC key was pressed or the window was closed
		while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0);

		//We want to clear up any memory we can
		for (int i = 0; i < models.size(); i++) {
			models[i].destroy();
		}

		//Use this to try and free up some memory
		std::vector<Model>().swap(models);
		std::vector<glm::vec3>().swap(modelLocations);

		glfwDestroyWindow(window);

		glfwTerminate();

		std::cout << "Type \"quit\" to exit out of the application, enter anything else to prompted to load another file" << std::endl;
		std::getline(std::cin, userInput);

	//keep going until the user inputs quit
	} while (!userInput._Equal("quit"));

}

