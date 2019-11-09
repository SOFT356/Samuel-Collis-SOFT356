#pragma once

#include <glm/glm.hpp> 
#include <GL/glew.h>

#include <string>
#include <vector>
#include <iostream>

class Model
{

public:
	//Vertex info
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;

	//Indice info
	std::vector<GLuint> vertexIndices;
	std::vector<GLuint> textureIndices;
	std::vector<GLuint> normalIndices;

	//how the object is rotated
	glm::vec3 rotation;
	//the location of the object
	glm::vec3 location;
	//the size of the object
	GLfloat scaleFactor;

	bool hasTexture = false;
	std::string textureLocation;

	bool createdSuccessfully = false;

	//Initialise
	void init(GLuint program);

	//Draw model
	void draw();

	//Move the model
	void translate(GLfloat x, GLfloat y, GLfloat z);
	void translate(glm::vec3 position);

	//Change the size of the model
	void scale(GLfloat scale);

	//Rotate the model
	void rotate(GLfloat xdelta, GLfloat ydelta, GLfloat zdelta);
	void rotate(glm::vec3 rotation);
	
	//clear up memory of model
	void destroy();
	
	//Print the variables to the console
	void debug();
	
private: 

	void applyLighting();

	void bindTexture();

};

