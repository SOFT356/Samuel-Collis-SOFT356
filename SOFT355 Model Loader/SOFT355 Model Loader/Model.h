#pragma once
#include <glm/glm.hpp> 
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include <string>
#include <vector>

#include "Vertex.h"
#include "ShaderLoader.h"

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

	enum VAO_IDs { Triangles, Indices, Colours, Tex, NumVAOs = 1 };
	enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
	enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

	#define BUFFER_OFFSET(a) ((void*)(a))

	GLuint  VAOs[NumVAOs];
	GLuint  Buffers[NumBuffers];
	GLuint texture1;

	void init() {

		glGenVertexArrays(NumVAOs, VAOs);
		glBindVertexArray(VAOs[Triangles]);

		//at the moment we have no shaders
		ShaderInfo  shaders[] =
		{
			{ GL_NONE, NULL }
		};

		GLuint program = LoadShaders(shaders);
		glUseProgram(program);

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		GLuint textureBuffer;
		glGenBuffers(1, &textureBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
		glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);

		// creating the model matrix
		glm::mat4 model = glm::mat4(1.0f);

		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//rotate for now so we can see it's 3d
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

		// creating the view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

		// creating the projection matrix
		glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

		// Adding all matrices up to create combined matrix
		glm::mat4 mvp = projection * view * model;

		//adding the Uniform to the shader
		int mvpLoc = glGetUniformLocation(program, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		glEnableVertexAttribArray(vPosition);
		glEnableVertexAttribArray(cPosition);
		glEnableVertexAttribArray(tPosition);
	}

	void draw() {
		
		static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT);
		glFrontFace(GL_CW); glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glBindVertexArray(VAOs[Triangles]);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glDrawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT, 0);
	}

	void translate();

	void rotate();

	void transform();


};

