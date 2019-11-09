#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <GL\glew.h>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	// Euler Angles
	GLfloat yaw;
	GLfloat pitch;
	// Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	GLfloat lastX;
	GLfloat lastY;
	bool firstMouse = true;

	// Constructor with vectors
	Camera() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		yaw = -90.0f;
		pitch = 0.0f;
		movementSpeed = 2.5f;
		mouseSensitivity = 0.1f;
		zoom = 45.0f;
		updateCameraVectors();
	}


	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		float velocity = movementSpeed * deltaTime;
		if (direction == FORWARD)
			position += front * velocity;
		if (direction == BACKWARD)
			position -= front * velocity;
		if (direction == LEFT)
			position -= right * velocity;
		if (direction == RIGHT)
			position += right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, bool constrainPitch = true)
	{
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (pitch > 89.0f) {
				pitch = 89.0f;
			}
			else if (pitch < -89.0f) {
				pitch = -89.0f;
			}
				
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (zoom >= 1.0f && zoom <= 45.0f) {
			zoom -= yoffset;
		}
		else if (zoom <= 1.0f) {
			zoom = 1.0f;
		}
		else if (zoom >= 45.0f) {
			zoom = 45.0f;
		}
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		up = glm::normalize(glm::cross(right, front));
	}
};

