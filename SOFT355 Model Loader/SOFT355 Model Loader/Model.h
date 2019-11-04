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

	vector<GLuint> indices;

	enum VAO_IDs { Vertices, Indices, Colours, Tex, NumVAOs = 1 };
	enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
	enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

	GLuint  VAOs[NumVAOs];
	GLuint  Buffers[NumBuffers];
	GLuint texture1;

	void init();

	void draw();

	void translate();

	void rotate();

	void transform();


};

