#pragma once
#include <glm/glm.hpp> //includes GLM


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec4 Colour;
	glm::vec2 TexCoords;
};