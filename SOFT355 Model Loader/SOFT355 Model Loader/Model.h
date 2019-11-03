#pragma once
#include <glm/glm.hpp> 
#include "GL/glew.h"
#include <vector>
#include "Vertex.h"

using std::vector;


class Model
{
	vector<Vertex> vertices;
	vector<GLuint> indices;
	
	void draw() {

	}

	void loadFromFile() {

	}

	void init();

	void translate();

	void rotate();

	void transform();


};

