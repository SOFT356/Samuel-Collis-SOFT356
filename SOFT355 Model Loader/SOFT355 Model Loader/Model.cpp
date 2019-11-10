#include "ShaderLoader.h"
#include "Model.h"
#include <glm/glm.hpp> 
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <vector>
#include <iostream>

#define BUFFER_OFFSET(a) ((void*)(a))

enum VAO_IDs { ModelVAO, NumVAOs = 1 };
enum Buffer_IDs { Vertices, Normals, Textures, VertexIndices, NumBuffers = 5 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint textureId;

GLuint usedProgram;

void Model::init(GLuint program) {

	usedProgram = program;
	glUseProgram(usedProgram);

	glGenVertexArrays(NumVAOs, VAOs);
	glGenBuffers(NumBuffers, Buffers);

	glBindVertexArray(VAOs[ModelVAO]);

	//Populate vertice and it's indices to the buffers
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Vertices]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[VertexIndices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(GLuint), vertexIndices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(Vertices, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));


	//Do the same for normals
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Normals]);
	glBufferStorage(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), 0);

	glVertexAttribPointer(Normals, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	//And for textures
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Textures]);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), textures.data(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(Textures, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	if (hasTexture) {
		bindTexture();
	}
	
	applyLighting();

	glEnableVertexAttribArray(Vertices);
	glEnableVertexAttribArray(Textures);
	glEnableVertexAttribArray(Normals);

	}

	void Model::draw() {

		static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		
		glClearBufferfv(GL_COLOR, 0, black);

		//Set the background to a dark red
		glClearColor(0.2f, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		//Only render faces we can see
	/*	glFrontFace(GL_CW);
		glCullFace(GL_BACK);*/
		glEnable(GL_CULL_FACE);

		//Create a model matrix
		glm::mat4 model = glm::mat4(1.0f);

		//Use the scale factor to scale the object
		model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

		//perform rotations 
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		// creating the view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, location);
		
		// creating the projection matrix
		glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

		// Adding all matrices up to create combined matrix
		glm::mat4 mv = view * model;

		//adding the Uniform to the shader
		int mvLoc = glGetUniformLocation(usedProgram, "mv_matrix");
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
		//adding the Uniform to the shader
		int pLoc = glGetUniformLocation(usedProgram, "p_matrix");
		glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAOs[ModelVAO]);
		if (hasTexture) {
			glBindTexture(GL_TEXTURE_2D, textureId);
		}
		
		glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);

	}

	void Model::translate(GLfloat x, GLfloat y, GLfloat z) {
		location.x = x;
		location.y = y;
		location.z = z;
	}
	 
	void Model::translate(glm::vec3 position) {
		location = position;
	}

	void Model::scale(GLfloat scale) {
		this->scaleFactor = scale;
	}

	void Model::rotate(GLfloat xdelta, GLfloat ydelta, GLfloat zdelta) {
		rotation.x = xdelta;
		rotation.y = ydelta;
		rotation.z = zdelta;
	}

	void Model::rotate(glm::vec3 rotation) {
		this->rotation = rotation;
	}

	void Model::destroy() {
		std::vector<glm::vec3>().swap(vertices);
		std::vector<glm::vec3>().swap(normals);
		std::vector<glm::vec2>().swap(textures);
		std::vector<GLuint>().swap(vertexIndices);
		std::vector<GLuint>().swap(textureIndices);
		std::vector<GLuint>().swap(normalIndices);
	}

	void Model::debug() {

		std::cout << "I have " << vertices.size() << " vertices" << std::endl;
		std::cout << "I have " << textures.size() << " textures" << std::endl;
		std::cout << "I have " << normals.size() << " normals" << std::endl;
		std::cout << "I have " << vertexIndices.size() << " vertex indices" << std::endl;
		std::cout << "I have " << textureIndices.size() << " texture indices" << std::endl;
		std::cout << "I have " << normalIndices.size() << " normal indices" << std::endl;


		for (int i = 0; i < vertices.size(); i++) {
			std::cout << vertices[i].x << "f," << vertices[i].y << "f," << vertices[i].z << "f," << std::endl;
		}

		std::cout << "Textures: " << std::endl;


		for (int i = 0; i < textures.size(); i++) {
			std::cout << textures[i].x << "f," << textures[i].y << "f," << std::endl;
		}

		std::cout << "Normals: " << std::endl;

		for (int i = 0; i < normals.size(); i++) {
			std::cout << normals[i].x << ".0f," << normals[i].y << ".0f," << normals[i].z << ".0f," << std::endl;
		}

		std::cout << "Vertex indices: " << std::endl;

		for (int i = 0; i < vertexIndices.size(); i++) {
			std::cout << vertexIndices[i] << ",";
		}


		std::cout << std::endl;
	}
 

	void Model::applyLighting() {
		//Create Ambient lighting and add to shader
		glm::vec4 ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		GLuint aLoc = glGetUniformLocation(usedProgram, "ambient");
		glUniform4fv(aLoc, 1, glm::value_ptr(ambient));

		//Create the light object and apply to shader
		glm::vec3 lightPos = glm::vec3(0.0f, 5.0f, 0.0f);
		GLuint dLightPosLoc = glGetUniformLocation(usedProgram, "lightPos");
		glUniform3fv(dLightPosLoc, 1, glm::value_ptr(lightPos));

		//Create light diffusion and add to shader
		glm::vec3 diffuseLight = glm::vec3(0.5f, 0.2f, 0.7f);
		GLuint dLightLoc = glGetUniformLocation(usedProgram, "dLight");
		glUniform3fv(dLightLoc, 1, glm::value_ptr(diffuseLight));

		//Create specular light and add to shader
		glm::vec3 specularLight = glm::vec3(0.7f);
		GLfloat shininess = 256; //128 is max value
		GLuint sLightLoc = glGetUniformLocation(usedProgram, "sLight");
		GLuint sShineLoc = glGetUniformLocation(usedProgram, "sShine");
		glUniform3fv(sLightLoc, 1, glm::value_ptr(specularLight));
		glUniform1fv(sShineLoc, 1, &shininess);
	}

	void Model::bindTexture() {

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		GLint width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char* data = stbi_load(textureLocation.c_str(), &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture" << std::endl;
		}

		stbi_image_free(data);

		glUniform1i(glGetUniformLocation(usedProgram, "textureId"), 0);
	}