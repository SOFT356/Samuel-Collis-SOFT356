#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>


Model loadFromObj(std::string file) {

	Model model;

	std::cout << "Creating from: " + file << std::endl;

	std::ifstream rfile;
	rfile.open(file);
	std::string line;

	std::vector<GLuint> vertexIndices, textureIndices, normalIndices;;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_textures;
	std::vector<glm::vec3> temp_normals;

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
				int matches = sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2],
					&vertexIndex[3], &uvIndex[3], &normalIndex[3]);

				if (matches != 12 && matches != 9) {
					std::cout << "There was a problem parsing the f data";
				}

				//so triangle 1
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);
				textureIndices.push_back(uvIndex[0] - 1);
				textureIndices.push_back(uvIndex[1] - 1);
				textureIndices.push_back(uvIndex[2] - 1);
				normalIndices.push_back(normalIndex[0] - 1);
				normalIndices.push_back(normalIndex[1] - 1);
				normalIndices.push_back(normalIndex[2] - 1);

				if (matches == 12) {
					//triangle 2
					vertexIndices.push_back(vertexIndex[2] - 1);
					vertexIndices.push_back(vertexIndex[3] - 1);
					vertexIndices.push_back(vertexIndex[0] - 1);
					textureIndices.push_back(uvIndex[0] - 1);
					textureIndices.push_back(uvIndex[1] - 1);
					textureIndices.push_back(uvIndex[2] - 1);
					normalIndices.push_back(normalIndex[0] - 1);
					normalIndices.push_back(normalIndex[1] - 1);
					normalIndices.push_back(normalIndex[2] - 1);
				}


			}

			/*std::cout << line << std::endl;*/
		}
	}

	/*std::cout << "Vertices: " << std::endl;

	for (int i = 0; i < temp_vertices.size(); i++) {
		std::cout << temp_vertices[i].x << "f," << temp_vertices[i].y << "f," << temp_vertices[i].z << "f," << std::endl;
	}

	std::cout << "Textures: " << std::endl;


	for (int i = 0; i < temp_textures.size(); i++) {
		std::cout << temp_textures[i].x << "f," << temp_textures[i].y << "f," << std::endl;
	}

	std::cout << "Normals: " << std::endl;

	for (int i = 0; i < temp_normals.size(); i++) {
		std::cout << temp_normals[i].x << ".0f," << temp_normals[i].y << ".0f," << temp_normals[i].z << ".0f," << std::endl;
	}

	std::cout << "Vertex indices: " << std::endl;

	for (int i = 0; i < vertexIndices.size(); i++) {
		std::cout << vertexIndices[i] << ",";
	}

	std::cout << "Texture indices: " << std::endl;

	for (int i = 0; i < textureIndices.size(); i++) {
		std::cout << textureIndices[i] << ",";
	}

	std::cout << "Normal indices: " << std::endl;

	for (int i = 0; i < normalIndices.size(); i++) {
		std::cout << normalIndices[i] << ",";
	}*/

	model.vertices = temp_vertices;
	model.textures = temp_textures;
	model.normals = temp_normals;
	model.vertexIndices = vertexIndices;
	model.textureIndices = textureIndices;
	model.normalIndices = normalIndices;

	//Performing a swap with an empty vector will free up the memory used
	vector<glm::vec3>().swap(temp_vertices);
	vector<glm::vec3>().swap(temp_normals);
	vector<glm::vec2>().swap(temp_textures);
	vector<GLuint>().swap(vertexIndices);
	vector<GLuint>().swap(textureIndices);
	vector<GLuint>().swap(normalIndices);

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


