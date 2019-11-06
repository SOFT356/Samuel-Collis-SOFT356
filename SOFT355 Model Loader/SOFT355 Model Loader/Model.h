#pragma once

#include <glm/glm.hpp> 
#include <GL/glew.h>

#include <string>
#include <vector>
#include <iostream>

using std::vector;

struct Texture {
	unsigned int id;
	std::string type;
};

class Model
{

public:
	vector<glm::vec3> vertices;
	vector<glm::vec2> textures;
	vector<glm::vec3> normals;

	vector<GLuint> vertexIndices;
	vector<GLuint> textureIndices;
	vector<GLuint> normalIndices;

	bool createdSuccessfully = false;

	void init();

	void draw(GLfloat xdelta, GLfloat ydelta, GLfloat zdelta, GLfloat scale);

	void translate();

	void rotate();

	void transform();


};

