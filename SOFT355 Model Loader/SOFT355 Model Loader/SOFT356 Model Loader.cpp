
#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp> //includes GLM
#include <iostream>
#include <string>
#include "ModelLoader.h"
#include "ShaderLoader.h"

GLfloat move = 1.0f;
GLfloat scaleIncrament = 0.005f;

glm::vec3 cameraLoc = glm::vec3(0.0f, 0.0f, -4.0f);

void processKeyEvents(GLFWwindow* window, glm::vec3 &rotation, GLfloat &scale) {
	
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
	
	if (scale < 0) {
		scale = 0.0000001;
	}
}


int main()
{
	std::string userInput, doWeContinue;
	Model model, model2;

	do {
		do {
			std::cout << "Please input the file location of the object or enter \"quit\" to close\n";
			std::cin >> userInput;

			if (userInput._Equal("quit")) {
				return 0;
			}
		
			model = loadFromFile(userInput);
			model2 = loadFromFile(userInput);
	
			if (!model.createdSuccessfully) {
				std::cout << "There was an issue loading your file, please try another" << std::endl;
			}

		} while (!model.createdSuccessfully);

		glfwInit();

		GLFWwindow* window = glfwCreateWindow(800, 600, userInput.c_str(), NULL, NULL);

		glfwMakeContextCurrent(window);
		glewInit();

		ShaderInfo  shaders[] =
		{
			{ GL_VERTEX_SHADER, "media/triangles.vert" },
			{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
			{ GL_NONE, NULL }
		};

		GLuint program = LoadShaders(shaders);
		
		model.init(program);
		model2.init(program);

		static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		GLfloat scale = 1.0f;
		
		do {

			model.translate(2, 2, -4);
			model.rotate(rotation);
			model.scale(scale);
			model.draw(cameraLoc);

			model2.translate(0, 0, -4);
			model2.rotate(rotation);
			model2.scale(scale);
			model2.draw(cameraLoc);

			processKeyEvents(window, rotation, scale);

			glfwSwapBuffers(window);
			glfwPollEvents();

			glClearBufferfv(GL_COLOR, 0, black);
			glClearColor(0.2f, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			
		} // Check if the ESC key was pressed or the window was closed
		while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0);

		model.destroy();
		model2.destroy();

		glfwDestroyWindow(window);

		glfwTerminate();

		std::cout << "Type \"quit\" to exit out of the application, enter anything else to prompted to load another file" << std::endl;
		std::cin >> doWeContinue;

	} while (!doWeContinue._Equal("quit"));

	

}

