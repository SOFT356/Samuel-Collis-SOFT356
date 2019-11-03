#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>


Model loadFromObj(std::string file) {

	Model model;

	std::cout << "Creating from: " + file;

	std::ifstream rfile;
	rfile.open(file);
	std::string line;

	std::vector< unsigned int > vertexIndices, textIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_textures;
	std::vector< glm::vec3 > temp_normals;

	std::string lineStart;

	if (rfile.is_open()) {
		while (std::getline(rfile, line)) {
			lineStart = line.substr(0, 2);

			if (lineStart._Equal("v ")) {
				glm::vec3 vertex;
				sscanf_s(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (lineStart._Equal("vt")) {
				glm::vec2 texture;
				sscanf_s(line.c_str(), "vt %f %f\n", &texture.x, &texture.y);
					temp_textures.push_back(texture);
			}
			else if (lineStart._Equal("vn")) {
				glm::vec3 normal;
				sscanf_s(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (lineStart._Equal("f ")) {
				std::string vertex1, vertex2, vertex3, vertex4;
				unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
				int matches = sscanf_s(line.c_str() , "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1], 
					&vertexIndex[2], &uvIndex[2], &normalIndex[2],
					&vertexIndex[3], &uvIndex[3], &normalIndex[3]);
				
				if (matches != 12) {
					std::cout << "There was a problem parsing the f data";
				}

				//As our obj has 4 faces, we need to construct 2 triangles instead
				//so triangle 1
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);

				//triangle 2
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[3]);

				textIndices.push_back(uvIndex[0]);
				textIndices.push_back(uvIndex[1]);
				textIndices.push_back(uvIndex[2]);

				textIndices.push_back(uvIndex[1]);
				textIndices.push_back(uvIndex[2]);
				textIndices.push_back(uvIndex[3]);

				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[3]);

			}

			std::cout << line << std::endl;
		}
	}

	//Indexing the data from the obj file
	for (int i = 0; i < vertexIndices.size(); i++) {
		int verInd = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[verInd - 1];
		model.vertices.push_back(vertex);
	}

	for (int i = 0; i < textIndices.size(); i++) {
		int textInd = textIndices[i];
		glm::vec2 text = temp_textures[textInd - 1];
		model.textures.push_back(text);
	}

	for (int i = 0; i < normalIndices.size(); i++) {
		int normalInd = normalIndices[i];
		glm::vec3 normal = temp_normals[normalInd - 1];
		model.normals.push_back(normal);
	}

	return model;
}


Model loadFromFile(std::string file) {
	
	std::string fileType = file.substr(file.length() - 4, file.length());

	Model newModel;

	if (fileType._Equal(".obj")) {
		newModel = loadFromObj(file);
	}
	else {
		std::cout << "Sorry this file type is not supported";
	}

	return newModel;

}


