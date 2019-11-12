#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>


Model loadFromObj(std::string file) {

	//Create the model to be returned
	Model model;

	//Log to the output that we're making the specified object
	std::cout << "Creating from: " + file << std::endl;

	//Create a file input stream
	std::ifstream rfile;
	rfile.open(file);

	//Instantiate a variable to hold each line of the string
	std::string line;

	//Create out variables to hold the parsed data untill the end
	std::vector<GLuint> vertexIndices, textureIndices, normalIndices, faceSize;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempTextures;
	std::vector<glm::vec3> tempNormals;

	//Create a string to hold the first two characters of each line just so we aren't making a method call each if check
	std::string lineStart;

	//While the file is open and has more contents 
	if (rfile.is_open()) {
		while (std::getline(rfile, line)) {
			lineStart = line.substr(0, 2);

			//V indicates a vertex
			if (lineStart._Equal("v ")) {
				glm::vec3 vertex;
				sscanf_s(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				tempVertices.push_back(vertex);
			}
			//vt indicates a texture vertex
			else if (lineStart._Equal("vt")) {
				glm::vec2 texture;
				sscanf_s(line.c_str(), "vt %f %f\n", &texture.x, &texture.y);
				tempTextures.push_back(texture);
			}
			//vn indicates a vertex normal
			else if (lineStart._Equal("vn")) {
				glm::vec3 normal;
				sscanf_s(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
				tempNormals.push_back(normal);
			}
			//f indicates a face
			else if (lineStart._Equal("f ")) {
				unsigned int vertexIndex[4], textureIndex[4], normalIndex[4];
				int matches = sscanf_s(line.c_str(),
					"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &textureIndex[0], &normalIndex[0],
					&vertexIndex[1], &textureIndex[1], &normalIndex[1],
					&vertexIndex[2], &textureIndex[2], &normalIndex[2],
					&vertexIndex[3], &textureIndex[3], &normalIndex[3]);


				//Obj files support n-agons, but in the majority of the time it is either a triangle or a quadrilateral
				//meaning that we should have either 3x3 or 4x3 of f/f/f per line 
				if (matches != 12 && matches != 9) {
					std::cout << "There was a problem parsing the f data" << std::endl;
					model.createdSuccessfully = false;
					return model;
				}

				//so for triangle 1 we need these
				//we subtract 1 from each index as arrays start from 0 in cpp
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);

				normalIndices.push_back(normalIndex[0] - 1);
				normalIndices.push_back(normalIndex[1] - 1);
				normalIndices.push_back(normalIndex[2] - 1);

				textureIndices.push_back(textureIndex[0] - 1);
				textureIndices.push_back(textureIndex[1] - 1);
				textureIndices.push_back(textureIndex[2] - 1);

				if (matches == 12) {

					//and for triangle 2 we need to add the final vertex of th
					vertexIndices.push_back(vertexIndex[3] - 1);
					textureIndices.push_back(textureIndex[3] - 1);
					normalIndices.push_back(normalIndex[3] - 1);

					//if we have 12 matches then we have a quadrilateral
					faceSize.push_back(4);
				}
				else {
					//else we just have a triangle
					faceSize.push_back(3);
				}

			}

		}
	}

	//We want to push all of temp vertices into the object
	for (int i = 0; i < vertexIndices.size(); i++) {
		int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = tempVertices[vertexIndex];
		model.vertices.push_back(vertex);
	}


	for (int i = 0; i < textureIndices.size(); i++) {
		//same as above but shorthand
		model.textures.push_back(tempTextures[textureIndices[i]]);
	}
	//model.textures = temp_textures;

	for (int i = 0; i < normalIndices.size(); i++) {
		model.normals.push_back(tempNormals[normalIndices[i]]);
	}


	int h = 0;

	//We take the value from the facesize and incrament by it to make sure we read
	//and construct objects correctly that used mixed n-gon faces like the low poly
	//boat
	for (int i = 0; i < model.vertices.size(); i += faceSize[h]) {
		model.vertexIndices.push_back(i + 0);
		model.vertexIndices.push_back(i + 1);
		model.vertexIndices.push_back(i + 2);

		if (faceSize[h] == 4) {
			model.vertexIndices.push_back(i + 2);
			model.vertexIndices.push_back(i + 3);
			model.vertexIndices.push_back(i + 0);
		}

		if (i + faceSize[h] != model.vertices.size()) {
			h++;
		}

	}

	rfile.close();

	//Look for a material file
	file = file.substr(0, file.length() - 3) + "mtl";

	rfile.open(file);

	//if the file is not open then there is no material file for the object
	if (rfile.is_open()) {
		while (std::getline(rfile, line)) {
			lineStart = line.substr(0, 6);
			if (lineStart._Equal("map_Kd")) {
				model.textureLocation = line.substr(line.find_first_of(" ") + 1, line.length());
				//we want to store the texture location in the same format that the user inputted the info in
				//this means that we will use the same file delimiter. This is done as windows and unix systems
				//use a different directional slash to indicate folder structure
				char fileDelimiter = file.find("\\") != std::string::npos ? '\\' : '/';
				model.textureLocation = file.substr(0, file.find_last_of(fileDelimiter) + 1) + model.textureLocation;
				std::cout << model.textureLocation << std::endl;
				model.hasTexture = true;
			}
		}

	}

	rfile.close();

	//Performing a swap with an empty vector will free up the memory used
	std::vector<glm::vec3>().swap(tempVertices);
	std::vector<glm::vec3>().swap(tempNormals);
	std::vector<glm::vec2>().swap(tempTextures);
	std::vector<GLuint>().swap(vertexIndices);
	std::vector<GLuint>().swap(textureIndices);
	std::vector<GLuint>().swap(normalIndices);

	//If there are no vertices then we have probably created the object incorrectly
	if (model.vertices.size() == 0) {
		model.createdSuccessfully = false;
	}
	else {
		model.createdSuccessfully = true;
	}

	return model;
}


Model loadFromFile(std::string file) {

	if (file.length() < 4) {
		return Model();
	}

	std::string fileType = file.substr(file.length() - 4, file.length());

	Model newModel;

	if (fileType._Equal(".obj")) {
		newModel = loadFromObj(file);
	}
	else {
		std::cout << "Sorry this file type is not supported" << std::endl;
	}

	return newModel;

}


