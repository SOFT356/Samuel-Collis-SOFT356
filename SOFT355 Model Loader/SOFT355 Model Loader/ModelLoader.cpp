#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>

//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

Model loadFromObj(std::string file) {
	Model model;

	std::cout << "Creating from: " + file;

	std::ifstream rfile;
	rfile.open(file);
	std::string line;

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
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
				glm::vec2 uv;
				sscanf_s(line.c_str(), "vt %f %f\n", &uv.x, &uv.y);
					temp_uvs.push_back(uv);
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

				
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);

				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[3]);
				vertexIndices.push_back(vertexIndex[4]);

				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);

				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[3]);
				uvIndices.push_back(uvIndex[4]);

				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[3]);
				normalIndices.push_back(normalIndex[4]);

			}

			std::cout << line << std::endl;
		}
	}

	model.vertices = temp_vertices;
	model.uvs = temp_uvs;
	model.normals = temp_normals;

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


