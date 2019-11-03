#pragma once
#include <glm/glm.hpp> 
#include "GL/glew.h"
#include <vector>
#include "Vertex.h"
#include <string>

using std::vector;

struct Texture {
	unsigned int id;
	std::string type;
};

class Model
{
public:
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;

	void draw() {
		
		
	}

	void loadFromFile() {

	}

	void init();

	void translate();

	void rotate();

	void transform();


};

