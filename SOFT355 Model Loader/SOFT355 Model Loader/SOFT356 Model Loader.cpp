
#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp> //includes GLM
#include <iostream>
#include <string>
#include "ModelLoader.h"
#include "ShaderLoader.h"

int main()
{
	std::string userInput, doWeContinue;
	bool debug = false; 
	bool wireFrame = false;
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

		GLfloat xdelta = 0.0f;
		GLfloat ydelta = 0.0f;
		GLfloat zdelta = 0.0f;
		GLfloat scale = 1.0f;

		GLfloat move = 1.0f;
		GLfloat scaleIncrament = 0.005f;

		if (debug) {
			model.debug();
		}
		
		do {
			// uncomment to draw only wireframe 
			if (wireFrame) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			
			model.translate(0, 0, -4);
			model.rotate(xdelta, ydelta, zdelta);
			model.scale(scale);
			model.draw();

			model2.translate(0, -2, -4);
			model2.rotate(xdelta, ydelta, zdelta);
			model2.scale(scale * 2);
			model2.draw();

			glfwSwapBuffers(window);
			glfwPollEvents();

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				ydelta -= move;
			}

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				ydelta += move;
			}

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				xdelta -= move;
			}

			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				xdelta += move;
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


		} // Check if the ESC key was pressed or the window was closed
		while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0);

		model.destroy();
		model2.destroy();

		glfwDestroyWindow(window);

		glfwTerminate();

		std::cout << "Type \"quit\" to exit out of the application, enter anything else to prompted to load another file" << std::endl;
		std::cin >> doWeContinue;

		debug = false;
		wireFrame = false;

	} while (!doWeContinue._Equal("quit"));

	

}
